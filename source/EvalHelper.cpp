#include "cga/EvalHelper.h"
#include "cga/RelativeFloat.h"
#include "cga/Variant.h"
#include "cga/EvalExpr.h"

#include <cgac/Expression.h>

#include <assert.h>

namespace cga
{

bool EvalHelper::SetPropVal(rttr::property prop, rttr::instance obj, 
                            const Variant& val)
{
    auto type = prop.get_type();
    if (type == rttr::type::get<float>())
    {
        assert(val.type == VarType::Float);
        auto succ = prop.set_value(obj, val.f);
        assert(succ);
    }
    else if (type == rttr::type::get<const char*>()
          || type == rttr::type::get<std::string>())
    {
        assert(val.type == VarType::String);
        auto succ = prop.set_value(obj, static_cast<const char*>(val.p));
        assert(succ);
    }
    else if (type == rttr::type::get<RelativeFloat>())
    {
        assert(val.type == VarType::Float);
        auto sv = prop.get_value(obj).get_value<RelativeFloat>();
        sv.value = val.f;
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
EvalHelper::ResolveSizeVal(const cgac::ExprNodePtr& expr, RelativeFloat& out_flt,
                           std::string& out_str)
{
    Variant var;

    switch (expr->op)
    {
    case cgac::OP_RELATIVE:
    {
        out_flt.relative = true;
        var = EvalExpr::Eval(expr->kids[0]);
    }
        break;

    default:
    {
        out_flt.relative = false;
        var = EvalExpr::Eval(expr);
    }
    }

    switch (var.type)
    {
    case VarType::Float:
        out_flt.value = var.f;
        break;
    case VarType::String:
        out_str = static_cast<const char*>(var.p);
        break;
    default:
        assert(0);
    }

    return var.type;
}

}