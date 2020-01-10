#include "cga/Variant.h"

#include <limits>

#include <assert.h>

namespace cga
{

Variant::Variant()
    : type(VarType::Invalid)
{
}

Variant::Variant(VarType type)
    : type(type)
{
}

Variant::Variant                                           (float f)
    : type(VarType::Float)
    , f(f)
{
}

Variant::Variant(const char* str)
    : type(VarType::String)
    , p(str)
{
}

//Variant::Variant(VarType type, const void* p)
//    : type(type)
//    , p(p)
//{
//}

bool Variant::ToBool() const
{
    switch (type)
    {
    case VarType::Float:
        return fabs(f) > std::numeric_limits<float>::epsilon();
    case VarType::String:
        return true;
    default:
        assert(0);
        return false;
    }
}

float Variant::ToFloat() const
{
    switch (type)
    {
    case VarType::Float:
        return f;
    default:
        assert(0);
        return 0;
    }
}

}