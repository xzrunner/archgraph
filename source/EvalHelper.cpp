#include "ce/EvalHelper.h"
#include "ce/RelativeFloat.h"
#include "ce/Variant.h"
#include "ce/EvalExpr.h"

#include <cgac/Expression.h>

#include <assert.h>

namespace ce
{

bool EvalHelper::SetPropVal(rttr::property prop, rttr::instance obj,
                            const VarPtr& val)
{
    auto type = prop.get_type();
    if (type == rttr::type::get<float>())
    {
        auto succ = prop.set_value(obj, check_float(val));
        assert(succ);
    }
    else if (type == rttr::type::get<const char*>()
          || type == rttr::type::get<std::string>())
    {
        auto succ = prop.set_value(obj, check_string(val));
        assert(succ);
    }
    else if (type == rttr::type::get<RelativeFloat>())
    {
        auto sv = prop.get_value(obj).get_value<RelativeFloat>();
        sv.value = check_float(val);
        auto succ = prop.set_value(obj, sv);
        assert(succ);
    }
    else
    {
        assert(0);
        return false;
    }
    return true;
}

VarType
EvalHelper::ResolveSizeVal(const cgac::ExprNodePtr& expr, const EvalContext& ctx,
                           RelativeFloat& out_flt, std::string& out_str)
{
    VarPtr var;

    switch (expr->op)
    {
    case cgac::OP_RELATIVE:
    {
        out_flt.relative = true;
        var = EvalExpr::Eval(expr->kids[0], ctx);
    }
        break;

    default:
    {
        out_flt.relative = false;
        var = EvalExpr::Eval(expr, ctx);
    }
    }

    assert(var);
    switch (var->Type())
    {
    case VarType::Float:
        out_flt.value = std::static_pointer_cast<FloatVar>(var)->GetValue();
        break;
    case VarType::String:
        out_str = std::static_pointer_cast<StringVar>(var)->GetValue();
        break;
    default:
        assert(0);
    }

    return var->Type();
}

}