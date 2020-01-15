#include "cga/EvalHelper.h"
#include "cga/RelativeFloat.h"
#include "cga/Variant.h"
#include "cga/EvalExpr.h"

#include <cgac/Expression.h>

#include <assert.h>

namespace cga
{

bool EvalHelper::SetPropVal(rttr::property prop, rttr::instance obj,
                            const VarPtr& val)
{
    auto type = prop.get_type();
    if (type == rttr::type::get<float>())
    {
        assert(val->Type() == VarType::Float);
        auto succ = prop.set_value(obj, val->ToFloat());
        assert(succ);
    }
    else if (type == rttr::type::get<const char*>()
          || type == rttr::type::get<std::string>())
    {
        assert(val->Type() == VarType::String);
        auto succ = prop.set_value(obj, val->ToString());
        assert(succ);
    }
    else if (type == rttr::type::get<RelativeFloat>())
    {
        assert(val->Type() == VarType::Float);
        auto sv = prop.get_value(obj).get_value<RelativeFloat>();
        sv.value = val->ToFloat();
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
        out_flt.value = var->ToFloat();
        break;
    case VarType::String:
        out_str = var->ToString();
        break;
    default:
        assert(0);
    }

    return var->Type();
}

}