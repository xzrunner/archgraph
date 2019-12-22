#pragma once

#include <cgac/typedef.h>

#include <memory>

namespace cga
{

class EvalExpr
{
public:
    enum class VarType
    {
        Invalid = 0,

        Float,
        String,
    };

    struct Variant
    {
        Variant();
        Variant(VarType type);
        explicit Variant(float f);
        explicit Variant(VarType type, const void* p);

        bool ToBool() const;
        float ToFloat() const;

        VarType type = VarType::Invalid;

        union
        {
            float       f;
            const void* p;
        };
    };

public:
    static Variant Eval(const cgac::ExprNodePtr& expr);

}; // EvalExpr

}