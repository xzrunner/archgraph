#include "cga/node/Split.h"
#include "cga/Geometry.h"
#include "cga/EvalExpr.h"
#include "cga/Variant.h"

#include <sm_const.h>
#include <halfedge/Polyhedron.h>
#include <halfedge/Utility.h>
#include <cgac/Expression.h>

namespace cga
{
namespace node
{

void Split::OnParmChanged(const std::string& parm_name)
{
    if (parm_name == ParmNames[static_cast<int>(Parm::Parts)]) {
        SetupExports();
    }
}

void Split::Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
                    const EvalContext& ctx)
{
    assert(in.size() == 1);
    if (!in[0]) {
        out.resize(m_parts.size(), nullptr);
        return;
    }

    for (auto& part : m_parts) {
        if (part.IsInvalid()) {
            out.resize(m_parts.size(), nullptr);
            return;
        }
    }

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

    auto prev_geo = in[0]->GetPoly()->GetTopoPoly();

    CutContext cut_ctx(normal, prev_geo);
    float begin, end;
    auto& aabb = in[0]->GetPoly()->GetTopoPoly()->GetAABB();
    switch (m_axis)
    {
    case Axis::X:
        begin = aabb.xmin;
        end = aabb.xmax;
        break;
    case Axis::Y:
        begin = aabb.ymin;
        end = aabb.ymax;
        break;
    case Axis::Z:
        begin = aabb.zmin;
        end = aabb.zmax;
        break;
    }

    out = CutGeo(begin, end, cut_ctx, m_parts, m_duplicate);
}

void Split::Setup(const std::vector<cgac::ExprNodePtr>& parms,
                  const Rule::CompoundSel& selectors, const EvalContext& ctx)
{
    assert(parms.size() == 1);
    auto var = EvalExpr::Eval(parms[0]);
    assert(var && var->Type() == VarType::String);
    auto type = rttr::type::get<Split::Axis>().get_enumeration()
        .name_to_value(var->ToString()).get_value<Split::Axis>();
    SetAxis(type);

    m_parts.clear();
    m_parts.reserve(selectors.sels.size());
    for (auto& sel : selectors.sels) {
        m_parts.push_back(SelectorToPart(sel));
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

std::vector<GeoPtr>
Split::CutGeo(float begin, float end, CutContext& ctx, const std::vector<Part>& parts, bool duplicate)
{
    std::vector<GeoPtr> result;
    if (duplicate)
    {
        size_t flt_num = 0;
        for (auto& p : parts) {
            if (p.HasFloat()) {
                flt_num++;
            }
        }

        if (flt_num > 0)
        {
            float tot_size = 0;
            for (auto& p : parts) {
                tot_size += p.CalcSize().first;
            }
            if (tot_size > 0)
            {
                const auto tot_len = end - begin;
                size_t num = 0;
                if (flt_num == parts.size()) {
                    num = static_cast<int>(std::floor(tot_len / tot_size));
                } else {
                    num = static_cast<int>(std::ceil(tot_len / tot_size));
                }
                assert(num > 0);
                const auto dist = tot_len / num;

                for (size_t i = 0; i < num; ++i)
                {
                    auto geos = CutGeoNoRepeat(begin, begin + dist, ctx, parts);
                    std::copy(geos.begin(), geos.end(), std::back_inserter(result));
                }
                assert(begin == end);
                //begin = end;
            }
            else
            {
                result = CutGeoNoRepeat(begin, end, ctx, parts);
                assert(begin == end);
            }
        }
        else
        {
            while (begin < end) {
                auto geos = CutGeoNoRepeat(begin, end, ctx, parts);
                std::copy(geos.begin(), geos.end(), std::back_inserter(result));
            }
        }

        assert(result.size() % parts.size() == 0);
    }
    else
    {
        result = CutGeoNoRepeat(begin, end, ctx, parts);
        assert(result.size() == parts.size());
    }

    return result;
}

std::vector<GeoPtr>
Split::CutGeoNoRepeat(float& begin, float end, CutContext& ctx, const std::vector<Part>& parts)
{
    if (begin >= end) {
        return std::vector<GeoPtr>();
    }

    std::vector<std::pair<float, bool>> sizes_with_type;
    sizes_with_type.reserve(parts.size());
    float tot_absolute = 0.0f;
    float tot_floating = 0.0f;
    for (auto& p : parts)
    {
        if (p.IsInvalid()) {
            return std::vector<GeoPtr>();
        }

        auto sz = p.CalcSize();
        sizes_with_type.push_back(sz);
        if (sz.second) {
            tot_absolute += sz.first;
        } else {
            tot_floating += sz.first;
        }
    }
    assert(parts.size() == sizes_with_type.size());

    std::vector<float> absolute_sizes;
    absolute_sizes.resize(parts.size(), 0);
    const auto tot_len = end - begin;

    float tot_relative = 0.0f;
    for (auto& p : parts) {
        if (p.size_type == SizeType::Relative) {
            tot_relative += p.size;
        }
    }

    float left = tot_len;
    for (size_t i = 0, n = parts.size(); i < n; ++i)
    {
        const auto& size = sizes_with_type[i].first;
        const auto& is_absolute = sizes_with_type[i].second;
        if (is_absolute)
        {
            if (size < left) {
                absolute_sizes[i] = size;
            } else {
                absolute_sizes[i] = left;
            }
        }
        else
        {
            if (tot_absolute >= tot_len)
            {
                absolute_sizes[i] = 0;
            }
            else
            {
                switch (parts[i].size_type)
                {
                case SizeType::Relative:
                    assert(tot_relative > 0);
                    absolute_sizes[i] = std::min(tot_len * parts[i].size, left);
                    break;
                case SizeType::Floating:
                case SizeType::None:
                    assert(tot_floating > 0);
                    absolute_sizes[i] = size * (tot_len - tot_absolute) / tot_floating;
                    break;
                default:
                    assert(0);
                }
            }
        }

        left -= absolute_sizes[i];
        if (fabs(left) < SM_LARGE_EPSILON) {
            left = 0;
        }
        assert(left >= 0);
        if (left == 0) {
            break;
        }
    }

    std::vector<GeoPtr> geos;

    assert(absolute_sizes.size() == parts.size());
    for (size_t i = 0, n = parts.size(); i < n; ++i)
    {
        auto& p = parts[i];
        auto& sz = absolute_sizes[i];
        if (p.size_type == SizeType::None)
        {
            std::vector<GeoPtr> children;
            assert(!p.children.empty());
            children = CutGeo(begin, begin + sz, ctx, p.children, p.repeat);
            begin = begin + sz;

            geos.push_back(std::make_shared<Geometry>(children));
        }
        else
        {
            begin += sz;
            if (fabs(end - begin) < SM_LARGE_EPSILON) {
                begin = end;
            }
            auto poly = CutSingle(begin, ctx);
            if (!poly) {
                geos.push_back(nullptr);
            } else {
                geos.push_back(std::make_shared<Geometry>(std::make_shared<pm3::Polytope>(poly)));
            }
        }
    }

    assert(geos.size() == parts.size());
    return geos;
}

Split::Part Split::SelectorToPart(const Rule::SelPtr& selector)
{
    Split::Part part;
    part.repeat = selector->duplicate;

    switch (selector->GetType())
    {
    case Rule::Selector::Type::Single:
    {
        auto expr = std::static_pointer_cast<Rule::SingleSel>(selector)->head;
        switch (expr->op)
        {
        case cgac::OP_RELATIVE:
        {
            part.size_type = Split::SizeType::Relative;
            auto var = EvalExpr::Eval(expr->kids[0]);
            assert(var && var->Type() == VarType::Float);
            part.size = var->ToFloat();
        }
            break;

        case cgac::OP_COMP:
        {
            part.size_type = Split::SizeType::Floating;
            auto var = EvalExpr::Eval(expr->kids[0]);
            assert(var && var->Type() == VarType::Float);
            part.size = var->ToFloat();
        }
            break;

        default:
        {
            part.size_type = Split::SizeType::Absolute;
            auto var = EvalExpr::Eval(expr);
            assert(var && var->Type() == VarType::Float);
            part.size = var->ToFloat();
        }
        }
    }
        break;
    case Rule::Selector::Type::Compound:
    {
        auto comp_sel = std::static_pointer_cast<Rule::CompoundSel>(selector);
        for (auto& csel : comp_sel->sels) {
            part.children.push_back(SelectorToPart(csel));
        }
    }
        break;
    default:
        assert(0);
    }

    return part;
}

he::PolyhedronPtr
Split::CutSingle(float pos, CutContext& ctx)
{
    if (pos == ctx.last_pos) {
        return nullptr;
    }
    ctx.last_pos = pos;

    sm::Plane plane(ctx.normal, -pos);
    ctx.curr_poly->Clip(plane, he::Polyhedron::KeepType::KeepAll, true);

    std::vector<he::Polyhedron::Face> up, down;
    for (auto& face : ctx.curr_poly->GetFaces())
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

    auto ret = std::make_shared<he::Polyhedron>(down);
    ctx.curr_poly = std::make_shared<he::Polyhedron>(up);
    return ret;
}

//////////////////////////////////////////////////////////////////////////
// struct Split::Part
//////////////////////////////////////////////////////////////////////////

Split::Part::
Part(SizeType size_type, float size, bool repeat)
    : size_type(size_type)
    , size(size)
    , repeat(repeat)
{
}

bool Split::Part::
operator == (const Part& p) const
{
    return size_type == p.size_type
        && size      == p.size
        && repeat    == p.repeat
        && children  == p.children;
}

std::pair<float, bool> Split::Part::
CalcSize() const
{
    float ret_size = 0.0f;
    bool is_absolute = false;
    switch (size_type)
    {
    case SizeType::Absolute:
        ret_size = size;
        is_absolute = true;
        break;
    case SizeType::Relative:
        ret_size = 0;
        is_absolute = false;
        break;
    case SizeType::Floating:
        ret_size = size;
        is_absolute = false;
        break;
    case SizeType::None:
    {
        assert(!children.empty());
        is_absolute = true;
        for (auto& c : children)
        {
            auto cs = c.CalcSize();
            if (!cs.second) {
                is_absolute = false;
            }
            ret_size += cs.first;
        }
    }
        break;
    default:
        assert(0);
    }

    if (repeat) {
        is_absolute = false;
    }

    return std::make_pair(ret_size, is_absolute);
}

bool Split::Part::HasFloat() const
{
    switch (size_type)
    {
    case SizeType::Absolute:
    case SizeType::Relative:
        return false;
    case SizeType::Floating:
        return true;
    case SizeType::None:
    {
        bool ret = false;
        assert(!children.empty());
        for (auto& c : children) {
            if (c.HasFloat()) {
                ret = true;
                break;
            }
        }
        return ret;
    }
    default:
        assert(0);
        return false;
    }
}

}
}