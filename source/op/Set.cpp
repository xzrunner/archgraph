#include "cga/op/Set.h"
#include "cga/Geometry.h"
#include "cga/EvalExpr.h"
#include "cga/CGA.h"
#include "cga/Variant.h"
#include "cga/EvalContext.h"

#include <cgac/Parser.h>
#include <cgac/Expression.h>

namespace cga
{
namespace op
{

void Set::OnParmChanged(const std::string& parm_name)
{
    if (parm_name == ParmNames[static_cast<int>(Parm::Value)])
    {
        cgac::Parser parser(m_value.c_str(), CGA::Instance()->GetStringPool());
        m_val_expr = cgac::ExpressionParser::ParseExpression(parser);
    }
}

void Set::Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
                  const EvalContext& ctx)
{
    const_cast<EvalContext&>(ctx).ChangeVar(m_attr, m_val_expr);

    out.reserve(in.size());
    for (auto& geo : in) {
        out.push_back(std::make_shared<Geometry>(*geo));
    }
    for (auto& geo : out) {
        auto var = EvalExpr::Eval(m_val_expr, ctx, geo);
        geo->SetAttr(m_attr, var);
    }
}

void Set::Setup(const std::vector<cgac::ExprNodePtr>& parms,
                const Rule::CompoundSel& selectors, const EvalContext& ctx)
{
    assert(parms.size() == 2);
    auto attr_var = EvalExpr::EvalNoExpand(parms[0], ctx);
    m_attr = check_string(attr_var);

    m_val_expr = parms[1];
}

}
}