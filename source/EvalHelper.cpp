#include "cga/EvalHelper.h"

#include "cga/node/SetSize.h"

#include <assert.h>

namespace cga
{

bool EvalHelper::SetPropVal(rttr::property prop, rttr::instance obj,
                            const EvalExpr::Variant& val)
{
    auto type = prop.get_type();
    if (type == rttr::type::get<float>())
    {
        assert(val.type == EvalExpr::VarType::Float);
        auto succ = prop.set_value(obj, val.f);
        assert(succ);
    }
    else if (type == rttr::type::get<const char*>()
          || type == rttr::type::get<std::string>())
    {
        assert(val.type == EvalExpr::VarType::String);
        auto succ = prop.set_value(obj, static_cast<const char*>(val.p));
        assert(succ);
    }
    else if (type == rttr::type::get<node::SetSize::SizeValue>())
    {
        assert(val.type == EvalExpr::VarType::Float);
        auto sv = prop.get_value(obj).get_value<node::SetSize::SizeValue>();
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

}