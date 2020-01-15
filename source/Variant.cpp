#include "cga/Variant.h"

#include <assert.h>

namespace cga
{

bool check_bool(const std::shared_ptr<Variant>& var)
{
    assert(var && var->Type() == VarType::Boolean);
    return std::static_pointer_cast<BoolVar>(var)->GetValue();
}

float check_float(const std::shared_ptr<Variant>& var)
{
    assert(var && var->Type() == VarType::Float);
    return std::static_pointer_cast<FloatVar>(var)->GetValue();
}

std::string check_string(const std::shared_ptr<Variant>& var)
{
    assert(var && var->Type() == VarType::String);
    return std::static_pointer_cast<StringVar>(var)->GetValue();
}

}