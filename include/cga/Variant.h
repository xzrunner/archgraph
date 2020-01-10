#pragma once

#include "cga/VarType.h"

namespace cga
{

struct Variant
{
    Variant();
    Variant(VarType type);
    explicit Variant(float f);
    explicit Variant(const char* str);
    //explicit Variant(VarType type, const void* p);

    bool ToBool() const;
    float ToFloat() const;

    VarType type = VarType::Invalid;

    union
    {
        float       f;
        const void* p;
    };
};
}