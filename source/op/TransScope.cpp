#include "ce/op/TransScope.h"
#include "ce/Geometry.h"
#include "ce/EvalHelper.h"

#include <cgac/Expression.h>

namespace ce
{
namespace op
{

void TransScope::Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
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

    sm::vec3 offset(0, 0, 0);
    if (m_tx.relative) {
        offset.x = m_tx.value * aabb.Width();
    } else {
        offset.x = m_tx.value;
    }
    if (m_ty.relative) {
        offset.y = m_ty.value * aabb.Height();
    } else {
        offset.y = m_ty.value;
    }
    if (m_tz.relative) {
        offset.z = m_tz.value * aabb.Depth();
    } else {
        offset.z = m_tz.value;
    }
    if (offset == sm::vec3(0, 0, 0)) {
        return;
    }

    auto poly = geo->GetPoly();
    auto topo_poly = poly->GetTopoPoly();
    auto first_v = topo_poly->GetVerts().Head();
    auto curr_v = first_v;
    do {
        curr_v->position += offset;
        curr_v = curr_v->linked_next;
    } while (curr_v != first_v);
    topo_poly->UpdateAABB();
    poly->BuildFromTopo();
}

void TransScope::Setup(const std::vector<cgac::ExprNodePtr>& parms,
                       const Rule::CompoundSel& selectors, const EvalContext& ctx)
{
    assert(parms.size() == 3 && selectors.sels.size() == 0);

    std::string sval;
    if (EvalHelper::ResolveSizeVal(parms[0], ctx, m_tx, sval) == VarType::String) {
        m_exprs_map.insert({ "sx", sval });
    }
    if (EvalHelper::ResolveSizeVal(parms[1], ctx, m_ty, sval) == VarType::String) {
        m_exprs_map.insert({ "sy", sval });
    }
    if (EvalHelper::ResolveSizeVal(parms[2], ctx, m_tz, sval) == VarType::String) {
        m_exprs_map.insert({ "sz", sval });
    }
}

}
}