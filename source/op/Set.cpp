#include "ce/op/Set.h"
#include "ce/Geometry.h"
#include "ce/EvalExpr.h"
#include "ce/CE.h"
#include "ce/Variant.h"
#include "ce/EvalContext.h"

#include <cgac/Parser.h>
#include <cgac/Expression.h>

namespace ce
{
namespace op
{

void Set::OnParmChanged(const std::string& parm_name)
{
    if (parm_name == ParmNames[static_cast<int>(Parm::Value)])
    {
        cgac::Parser parser(m_value.c_str(), CE::Instance()->GetStringPool());
        m_val_expr = cgac::ExpressionParser::ParseExpression(parser);
    }
}

void Set::Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
                  const EvalContext& ctx)
{
    out.reserve(in.size());
    for (auto& geo : in)
    {
        auto var = EvalExpr::Eval(m_val_expr, ctx, geo);
        switch (var->Type())
        {
        case VarType::Boolean:
            const_cast<EvalContext&>(ctx).AddVar(EvalContext::Parm(m_attr,
                dag::Variable(std::static_pointer_cast<BoolVar>(var)->GetValue())
            ));
            break;
        case VarType::Float:
            const_cast<EvalContext&>(ctx).AddVar(EvalContext::Parm(m_attr,
                dag::Variable(std::static_pointer_cast<FloatVar>(var)->GetValue())
            ));
            break;
        case VarType::String:
            const_cast<EvalContext&>(ctx).AddVar(EvalContext::Parm(m_attr,
                dag::Variable(std::static_pointer_cast<StringVar>(var)->GetValue())
            ));
            break;
        default:
            assert(0);
        }

        out.push_back(std::make_shared<Geometry>(*geo));
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