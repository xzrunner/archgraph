#include "cga/op/Scale.h"
#include "cga/Geometry.h"
#include "cga/EvalHelper.h"

#include <cgac/Expression.h>

namespace cga
{
namespace op
{

void Scale::Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
                    const EvalContext& ctx)
{
    if (in.empty() || !in[0]) {
        out.resize(1, nullptr);
        return;
    }

    assert(in.size() == 1);
    auto prev_poly = in[0]->GetPoly();
    if (!prev_poly) {
        out.resize(1);
        out[0] = in[0];
        return;
    }

    auto geo = std::make_shared<Geometry>(*in[0]);
    out.resize(1);
    out[0] = geo;

    auto& aabb = prev_poly->GetTopoPoly()->GetAABB();

    sm::vec3 scale(1, 1, 1);
    if (m_sx.relative) {
        scale.x = m_sx.value;
    } else if (aabb.Width() != 0) {
        scale.x = m_sx.value / aabb.Width();
    }
    if (m_sy.relative) {
        scale.y = m_sy.value;
    } else if (aabb.Height() != 0) {
        scale.y = m_sy.value / aabb.Height();
    }
    if (m_sz.relative) {
        scale.z = m_sz.value;
    } else if (aabb.Depth() != 0) {
        scale.z = m_sz.value / aabb.Depth();
    }
    if (scale == sm::vec3(1, 1, 1)) {
        return;
    }

    auto poly = geo->GetPoly();
    auto topo_poly = poly->GetTopoPoly();
    auto first_v = topo_poly->GetVerts().Head();
    auto curr_v = first_v;
    do {
        curr_v->position = curr_v->position * scale;
        curr_v = curr_v->linked_next;
    } while (curr_v != first_v);
    topo_poly->UpdateAABB();
    poly->BuildFromTopo();
}

void Scale::Setup(const std::vector<cgac::ExprNodePtr>& parms,
                  const Rule::CompoundSel& selectors, const EvalContext& ctx)
{
    assert(parms.size() == 3 && selectors.sels.size() == 0);

    std::string sval;
    if (EvalHelper::ResolveSizeVal(parms[0], ctx, m_sx, sval) == VarType::String) {
        m_exprs_map.insert({ "sx", sval });
    }
    if (EvalHelper::ResolveSizeVal(parms[1], ctx, m_sy, sval) == VarType::String) {
        m_exprs_map.insert({ "sy", sval });
    }
    if (EvalHelper::ResolveSizeVal(parms[2], ctx, m_sz, sval) == VarType::String) {
        m_exprs_map.insert({ "sz", sval });
    }
}

}
}