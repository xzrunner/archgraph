#include "cga/node/Split.h"
#include "cga/Geometry.h"
#include "cga/EvalExpr.h"

#include <halfedge/Polyhedron.h>
#include <halfedge/Utility.h>
#include <cgac/Expression.h>

namespace cga
{
namespace node
{

void Split::Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
                    const EvalContext& ctx)
{
    assert(in.size() == 1);
    if (!in[0]) {
        out.resize(m_parts.size(), nullptr);
        return;
    }

    auto knife_pos = CalcKnifePos(in[0]);

    sm::vec3 normal;
    switch (m_axis)
    {
    case Axis::X:
        normal.Set(1, 0, 0);
        break;
    case Axis::Y:
        normal.Set(0, 1, 0);
        break;
    case Axis::Z:
        normal.Set(0, 0, 1);
        break;
    default:
        assert(0);
    }

    float last_pos = -1;

    auto prev_geo = in[0]->GetPoly()->GetTopoPoly();
    for (auto& pos : knife_pos)
    {
        if (pos == last_pos) {
            out.push_back(nullptr);
            continue;
        }
        last_pos = pos;

        sm::Plane plane(normal, -pos);
        prev_geo->Clip(plane, he::Polyhedron::KeepType::KeepAll, true);

        std::vector<he::Polyhedron::Face> up, down;
        for (auto& face : prev_geo->GetFaces())
        {
            auto st = he::Utility::CalcFacePlaneStatus(face, plane);
            switch (st)
            {
            case he::Utility::FaceStatus::Above:
                up.push_back(face);
                break;
            case he::Utility::FaceStatus::Below:
                down.push_back(face);
                break;
            case he::Utility::FaceStatus::Inside:
                up.push_back(face);
                down.push_back(face);
                break;
            default:
                assert(0);
            }
        }

        auto part = std::make_shared<he::Polyhedron>(down);
        out.push_back(std::make_shared<Geometry>(std::make_shared<pm3::Polytope>(part)));

        prev_geo = std::make_shared<he::Polyhedron>(up);
    }
}

void Split::Setup(const std::vector<cgac::ExprNodePtr>& parms,
                  const Rule::CompoundSel& selectors, const EvalContext& ctx)
{
    assert(parms.size() == 1);
    auto var = EvalExpr::Eval(parms[0]);
    assert(var.type == EvalExpr::VarType::String);
    auto type = rttr::type::get<Split::Axis>().get_enumeration()
        .name_to_value(static_cast<const char*>(var.p)).get_value<Split::Axis>();
    SetAxis(type);

    m_parts.clear();
    m_parts.reserve(selectors.sels.size());
    for (auto& sel : selectors.sels)
    {
        if (sel->GetType() != Rule::Selector::Type::Single) {
            continue;
        }

        auto expr = std::static_pointer_cast<Rule::SingleSel>(sel)->head;

        Split::Part part;
        switch (expr->op)
        {
        case cgac::OP_RELATIVE:
        {
            part.type = Split::SizeType::Relative;
            auto var = EvalExpr::Eval(expr->kids[0]);
            assert(var.type == EvalExpr::VarType::Float);
            part.size = var.f;
            part.repeat = false;
        }
            break;

        case cgac::OP_COMP:
        {
            part.type = Split::SizeType::Floating;
            auto var = EvalExpr::Eval(expr->kids[0]);
            assert(var.type == EvalExpr::VarType::Float);
            part.size = var.f;
            part.repeat = false;
        }
            break;

        default:
        {
            part.type = Split::SizeType::Absolute;
            auto var = EvalExpr::Eval(expr);
            assert(var.type == EvalExpr::VarType::Float);
            part.size = var.f;
            part.repeat = false;
        }
        }
        m_parts.push_back(part);
    }

    m_duplicate = selectors.duplicate;

    SetupExports();
}

void Split::SetupExports()
{
    m_exports.clear();
    m_exports.reserve(m_parts.size());
    for (size_t i = 0, n = m_parts.size(); i < n; ++i) {
        m_exports.push_back({ { NodeVarType::Any, "out" + std::to_string(i) } });
    }

    SetPortChanged(true);
}

std::vector<float> Split::CalcKnifePos(const GeoPtr& geo) const
{
    std::vector<float> sizes;
    sizes.resize(m_parts.size(), 0);

    float tot_len, start_pos;
    auto& aabb = geo->GetPoly()->GetTopoPoly()->GetAABB();
    switch (m_axis)
    {
    case Axis::X:
        tot_len = aabb.xmax - aabb.xmin;
        start_pos = aabb.xmin;
        break;
    case Axis::Y:
        tot_len = aabb.ymax - aabb.ymin;
        start_pos = aabb.ymin;
        break;
    case Axis::Z:
        tot_len = aabb.zmax - aabb.zmin;
        start_pos = aabb.zmin;
        break;
    }

    float tot_absolute = 0.0f;
    float tot_relative = 0.0f;
    float tot_floating = 0.0f;
    for (auto& part : m_parts) 
    {
        switch (part.type)
        {
        case Split::SizeType::Absolute:
            tot_absolute += part.size;
            break;
        case Split::SizeType::Relative:
            tot_relative += part.size;
            break;
        case Split::SizeType::Floating:
            tot_floating += part.size;
            break;
        default:
            assert(0);
        }
    }

    float left = tot_len;
    for (size_t i = 0, n = m_parts.size(); i < n; ++i)
    {
        float abs_sz = 0;

        auto& p = m_parts[i];
        switch (p.type)
        {
        case SizeType::Absolute:
            abs_sz = p.size;
            break;
        case SizeType::Relative:
            abs_sz = tot_len * p.size;
            break;
        case SizeType::Floating:
        {
            if (tot_absolute == 0 && tot_relative == 0 && tot_floating != 0)
            {
                abs_sz = tot_len * p.size / tot_floating;
            }
        }
            break;
        default:
            assert(0);
        }

        if (abs_sz <= left) {
            left -= abs_sz;
        } else {
            abs_sz = left;
            left = 0;
        }

        sizes[i] = abs_sz;

        if (left == 0) {
            break;
        }
    }

    std::vector<float> pos;
    float curr_pos = start_pos;
    for (size_t i = 0, n = sizes.size(); i < n ; ++i)
    {
        curr_pos += sizes[i];
        pos.push_back(curr_pos);
    }
    return pos;
}

}
}