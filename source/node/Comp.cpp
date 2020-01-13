#include "cga/node/Comp.h"
#include "cga/EvalExpr.h"
#include "cga/Geometry.h"
#include "cga/Variant.h"

#include <SM_Matrix.h>
#include <SM_Quaternion.h>
#include <SM_Calc.h>
#include <sm_const.h>
#include <halfedge/Utility.h>

namespace
{

int selector_offset(cga::node::Comp::Selector sel, cga::node::Comp::Selector start)
{
    return static_cast<int>(sel) - static_cast<int>(start);
}

}

namespace cga
{
namespace node
{

void Comp::Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
                   const EvalContext& ctx)
{
    // todo: only support faces now
    if (in.empty() || m_type != Type::Faces) {
        return;
    }

    assert(in.size() == 1);
    if (!in[0]) {
        out.resize(m_selectors.size(), nullptr);
        return;
    }
    for (auto& sel : m_selectors) {
        out.push_back(BuildComp(in[0], sel));
    }
}

void Comp::Setup(const std::vector<cgac::ExprNodePtr>& parms,
                 const Rule::CompoundSel& selectors, const EvalContext& ctx)
{
    assert(parms.size() == 1);
    auto var = EvalExpr::Eval(parms[0]);
    assert(var && var->Type() == VarType::String);
    auto type = rttr::type::get<Comp::Type>().get_enumeration()
        .name_to_value(var->ToString()).get_value<Comp::Type>();
    SetType(type);

    m_selectors.clear();
    m_selectors.reserve(selectors.sels.size());
    for (auto& sel : selectors.sels)
    {
        switch (sel->GetType())
        {
        case Rule::Selector::Type::Single:
        {
            auto var = EvalExpr::Eval(std::static_pointer_cast<Rule::SingleSel>(sel)->head);
            assert(var && var->Type() == VarType::String);
            auto sel = rttr::type::get<Comp::Selector>().get_enumeration()
                .name_to_value(var->ToString()).get_value<Comp::Selector>();
            m_selectors.push_back(sel);
        }
            break;
        default:
            assert(0);
        }
    }

    SetupExports();
}

void Comp::SetupExports()
{
    auto sel_enum = rttr::type::get<Comp::Selector>().get_enumeration();

    m_exports.clear();
    m_exports.reserve(m_selectors.size());
    for (auto& sel : m_selectors) {
        auto name = sel_enum.value_to_name(sel).to_string();
        m_exports.push_back({{ NodeVarType::Any, name }});
    }

    SetPortChanged(true);
}

GeoPtr Comp::BuildComp(const GeoPtr& geo, Selector sel)
{
    switch (sel)
    {
    case Selector::Front:
    case Selector::Back:
    case Selector::Left:
    case Selector::Right:
    case Selector::Top:
    case Selector::Bottom:
        return BuildFace6(geo, sel);

    case Selector::Border:
    case Selector::Inside:
        return BuildBorderInside(geo, sel);

    case Selector::Vertical:
    case Selector::Horizontal:
    case Selector::Aslant:
    case Selector::Nutant:
        return BuildNormalY(geo, sel);

    default:
        assert(0);
        return nullptr;
    }
}

GeoPtr Comp::BuildFace6(const GeoPtr& geo, Selector sel)
{
    assert(static_cast<int>(sel) >= static_cast<int>(Selector::Front)
        && static_cast<int>(sel) <= static_cast<int>(Selector::Bottom));
    if (static_cast<int>(sel) < static_cast<int>(Selector::Front) ||
        static_cast<int>(sel) > static_cast<int>(Selector::Bottom)) {
        return nullptr;
    }

    auto poly = geo->GetPoly();
    auto topo_poly = poly->GetTopoPoly();
    auto& faces = topo_poly->GetFaces();
    if (faces.empty()) {
        return nullptr;
    }

    sm::vec3 normals[6];
    normals[selector_offset(Selector::Front,  Selector::Front)].Set( 0,  0, -1);
    normals[selector_offset(Selector::Back,   Selector::Front)].Set( 0,  0,  1);
    normals[selector_offset(Selector::Left,   Selector::Front)].Set(-1,  0,  0);
    normals[selector_offset(Selector::Right,  Selector::Front)].Set( 1,  0,  0);
    normals[selector_offset(Selector::Top,    Selector::Front)].Set( 0,  1,  0);
    normals[selector_offset(Selector::Bottom, Selector::Front)].Set( 0, -1,  0);

    std::vector<he::Polyhedron::Face> dst_faces6[6];

    for (auto& face : faces)
    {
        assert(face.border);
        auto norm = he::Utility::CalcFaceNorm(face);
        int min_idx = -1;
        float min_angle = FLT_MAX;
        for (size_t i = 0; i < 6; ++i)
        {
            auto angle = sm::get_angle(sm::vec3(0, 0, 0), norm, normals[i]);
            if (angle < min_angle) {
                min_angle = angle;
                min_idx = i;
            }
        }
        assert(min_idx >= 0 && min_idx < 6);
        dst_faces6[min_idx].push_back(face);
    }

    auto& dst_faces = dst_faces6[selector_offset(sel, Selector::Front)];
    if (dst_faces.empty()) {
        return nullptr;
    }

    auto dst_topo = std::make_shared<he::Polyhedron>(dst_faces);
    auto dst_poly = std::make_shared<pm3::Polytope>(dst_topo);
    return std::make_shared<Geometry>(dst_poly);
}

GeoPtr Comp::BuildBorderInside(const GeoPtr& geo, Selector sel)
{
    assert(sel == Selector::Border || sel == Selector::Inside);
    if (sel != Selector::Border && sel != Selector::Inside) {
        return nullptr;
    }

    auto poly = geo->GetPoly();
    auto topo_poly = poly->GetTopoPoly();
    auto& faces = topo_poly->GetFaces();
    if (faces.empty()) {
        return nullptr;
    }

    if (faces.size() == 1)
    {
        if (sel == Selector::Border)
        {
            std::vector<he::Polyhedron::Face> dst_faces;
            dst_faces.push_back(faces[0]);
            auto dst_topo = std::make_shared<he::Polyhedron>(dst_faces);
            auto dst_poly = std::make_shared<pm3::Polytope>(dst_topo);
            return std::make_shared<Geometry>(dst_poly);
        }
        else
        {
            return nullptr;
        }
    }
    else
    {
        auto is_face_in_face = [](const he::Polyhedron::Face& in, const he::Polyhedron::Face& out) -> bool
        {
            auto norm_in = he::Utility::CalcFaceNorm(in);
            auto norm_out = he::Utility::CalcFaceNorm(out);
            assert(fabs(norm_in.Dot(norm_out) - norm_in.Length() * norm_out.Length()) < SM_LARGE_EPSILON);

            if (out.holes.empty() || !in.border) {
                return false;
            }

            auto loop2verts = [](const he::loop3& loop, const sm::mat4& mt) -> std::vector<sm::vec2>
            {
                std::vector<sm::vec3> verts3;
                he::Utility::LoopToVertices(loop, verts3);
                std::vector<sm::vec2> verts2;
                verts2.resize(verts3.size());
                for (size_t i = 0, n = verts3.size(); i < n; ++i) {
                    auto p = mt * verts3[i];
                    verts2[i] = sm::vec2(p.x, p.z);
                }
                return verts2;
            };

            auto mt2d = sm::mat4(sm::Quaternion::CreateFromVectors(norm_in, sm::vec3(0, 1, 0)));
            auto in_border = loop2verts(*in.border, mt2d);
            for (auto& hole : out.holes)
            {
                auto out_hole = loop2verts(*hole, mt2d);
                if (!sm::is_polygon_in_polygon(in_border, out_hole)) {
                    return false;
                }
            }

            return true;
        };

        for (size_t i = 0, n = faces.size(); i < n; ++i)
        {
            bool is_border = true;
            for (size_t j = 0; j < n; ++j)
            {
                if (j == i) {
                    continue;
                }
                if (!is_face_in_face(faces[j], faces[i])) {
                    is_border = false;
                }
            }

            if (is_border)
            {
                if (sel == Selector::Border)
                {
                    std::vector<he::Polyhedron::Face> dst_faces;
                    dst_faces.push_back(faces[i]);
                    auto dst_topo = std::make_shared<he::Polyhedron>(dst_faces);
                    auto dst_poly = std::make_shared<pm3::Polytope>(dst_topo);
                    return std::make_shared<Geometry>(dst_poly);
                }
                else
                {
                    std::vector<he::Polyhedron::Face> dst_faces;
                    for (size_t j = 0, n = faces.size(); j < n; ++j) {
                        if (i != j) {
                            dst_faces.push_back(faces[j]);
                        }
                    }
                    auto dst_topo = std::make_shared<he::Polyhedron>(dst_faces);
                    auto dst_poly = std::make_shared<pm3::Polytope>(dst_topo);
                    return std::make_shared<Geometry>(dst_poly);
                }
            }
        }
        return nullptr;
    }
}

GeoPtr Comp::BuildNormalY(const GeoPtr& geo, Selector sel)
{
    assert(sel == Selector::Vertical
        || sel == Selector::Horizontal
        || sel == Selector::Aslant
        || sel == Selector::Nutant);
    if (sel != Selector::Vertical &&
        sel != Selector::Horizontal &&
        sel != Selector::Aslant &&
        sel != Selector::Nutant) {
        return nullptr;
    }

    auto poly = geo->GetPoly();
    auto topo_poly = poly->GetTopoPoly();
    auto& faces = topo_poly->GetFaces();
    if (faces.empty()) {
        return nullptr;
    }

    std::vector<he::Polyhedron::Face> dst_normy[4];

    static const float EPSILON = 0.0001f;
    for (auto& face : faces)
    {
        auto norm = he::Utility::CalcFaceNorm(face);
        auto angle = sm::get_angle(sm::vec3(0, 0, 0), norm, sm::vec3(0, 1, 0)) * SM_RAD_TO_DEG - 90;
        if (angle > 78.75f && angle < 90 + EPSILON) {
            dst_normy[selector_offset(Selector::Horizontal, Selector::Vertical)].push_back(face);
        } else if (angle > 11.25f && angle <= 78.75f) {
            dst_normy[selector_offset(Selector::Aslant, Selector::Vertical)].push_back(face);
        } else if (angle > -11.25f && angle <= 11.25f) {
            dst_normy[selector_offset(Selector::Vertical, Selector::Vertical)].push_back(face);
        } else if (angle > -78.75f && angle <= -11.25f) {
            dst_normy[selector_offset(Selector::Nutant, Selector::Vertical)].push_back(face);
        } else if (angle > -90 - EPSILON && angle <= -78.75f) {
            dst_normy[selector_offset(Selector::Horizontal, Selector::Vertical)].push_back(face);
        } else {
            assert(0);
        }
    }

    auto& dst_faces = dst_normy[selector_offset(sel, Selector::Vertical)];
    if (dst_faces.empty()) {
        return nullptr;
    }

    auto dst_topo = std::make_shared<he::Polyhedron>(dst_faces);
    auto dst_poly = std::make_shared<pm3::Polytope>(dst_topo);
    return std::make_shared<Geometry>(dst_poly);
}

}
}