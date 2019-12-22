#include "cga/node/Comp.h"
#include "cga/EvalExpr.h"
#include "cga/Geometry.h"

#include <SM_Matrix.h>
#include <SM_Quaternion.h>
#include <SM_Calc.h>
#include <halfedge/Utility.h>

namespace cga
{
namespace node
{

void Comp::Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out)
{
    // todo: only support faces now
    if (m_type != Type::Faces) {
        return;
    }

    assert(in.size() == 1);
    for (auto& sel : m_selectors) {
        out.push_back(BuildComp(in[0], sel));
    }
}

void Comp::Setup(const std::vector<cgac::ExprNodePtr>& parms,
                 const std::vector<cgac::ExprNodePtr>& selectors,
                 const std::map<std::string, cgac::ExprNodePtr>& symbols)
{
    assert(parms.size() == 1);
    auto var = EvalExpr::Eval(parms[0]);
    assert(var.type == EvalExpr::VarType::String);
    auto type = rttr::type::get<Comp::Type>().get_enumeration()
        .name_to_value(static_cast<const char*>(var.p)).get_value<Comp::Type>();
    SetType(type);

    m_selectors.clear();
    m_selectors.reserve(selectors.size());
    for (auto& s : selectors)
    {
        auto var = EvalExpr::Eval(s);
        assert(var.type == EvalExpr::VarType::String);
        auto sel = rttr::type::get<Comp::Selector>().get_enumeration()
            .name_to_value(static_cast<const char*>(var.p)).get_value<Comp::Selector>();
        m_selectors.push_back(sel);
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
    auto poly = geo->GetPoly();
    auto topo_poly = poly->GetTopoPoly();
    auto& faces = topo_poly->GetFaces();
    if (faces.empty()) {
        return nullptr;
    }

    switch (sel)
    {
    case Selector::Border:
    case Selector::Inside:
    {
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
                assert(fabs(norm_in.x - norm_out.x) < FLT_EPSILON
                    && fabs(norm_in.y - norm_out.y) < FLT_EPSILON
                    && fabs(norm_in.z - norm_out.z) < FLT_EPSILON);

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
        break;
    default:
        assert(0);
        return nullptr;
    }
}

}
}