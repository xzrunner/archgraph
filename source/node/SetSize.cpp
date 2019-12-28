#include "cga/node/SetSize.h"
#include "cga/Geometry.h"

#include <cgac/Expression.h>

namespace cga
{
namespace node
{

void SetSize::Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
                      const EvalContext& ctx)
{
    assert(in.size() == 1);
    if (!in[0]) {
        return;
    }

    auto prev_poly = in[0]->GetPoly();
    if (!prev_poly) {
        out.resize(1);
        out[0] = in[0];
        return;
    }

    auto& aabb = prev_poly->GetTopoPoly()->GetAABB();

    sm::vec3 scale;
    scale.x = m_x.relative ? m_x.value : (m_x.value / aabb.Width());
    scale.y = m_y.relative ? m_y.value : (m_y.value / aabb.Height());
    scale.z = m_z.relative ? m_z.value : (m_z.value / aabb.Depth());
    if (scale == sm::vec3(1, 1, 1)) {
        return;
    }

    auto geo = std::make_shared<Geometry>(*in[0]);
    out.resize(1);
    out[0] = geo;

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

void SetSize::Setup(const std::vector<cgac::ExprNodePtr>& parms,
                    const std::vector<cgac::ExprNodePtr>& selectors, const EvalContext& ctx)
{
    assert(parms.size() == 3 && selectors.size() == 0);

    std::string sval;
    if (ResolveSizeVal(parms[0], m_x.relative, m_x.value, sval) == EvalExpr::VarType::String) {
        m_exprs_map.insert({ "x", sval });
    }
    if (ResolveSizeVal(parms[1], m_y.relative, m_y.value, sval) == EvalExpr::VarType::String) {
        m_exprs_map.insert({ "y", sval });
    }
    if (ResolveSizeVal(parms[2], m_z.relative, m_z.value, sval) == EvalExpr::VarType::String) {
        m_exprs_map.insert({ "z", sval });
    }
}

EvalExpr::VarType SetSize::ResolveSizeVal(const cgac::ExprNodePtr& expr,
                                          bool& relative, float& fval, std::string& sval)
{
    EvalExpr::Variant var;

    switch (expr->op)
    {
    case cgac::OP_RELATIVE:
    {
        relative = true;
        var = EvalExpr::Eval(expr->kids[0]);
    }
        break;

    default:
    {
        relative = false;
        var = EvalExpr::Eval(expr);
    }
    }

    switch (var.type)
    {
    case EvalExpr::VarType::Float:
        fval = var.f;
        break;
    case EvalExpr::VarType::String:
        sval = static_cast<const char*>(var.p);
        break;
    default:
        assert(0);
    }

    return var.type;
}

}
}