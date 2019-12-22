#include "cga/EvalExpr.h"

#include <cgac/Expression.h>

#define EVAL_V0                       \
auto v0 = Eval(expr->kids[0]);        \
if (v0.type == VarType::Invalid) {    \
    return Variant(VarType::Invalid); \
}

#define EVAL_V1                       \
auto v1 = Eval(expr->kids[1]);        \
if (v1.type == VarType::Invalid) {    \
    return Variant(VarType::Invalid); \
}

namespace cga
{

EvalExpr::Variant
EvalExpr::Eval(const cgac::ExprNodePtr& expr)
{
    switch (expr->op)
    {
    case cgac::OP_COMMA:
        return Eval(expr->kids[1]);
    case cgac::OP_ASSIGN:
        return Eval(expr->kids[1]);
    case cgac::OP_OR:
    {
        EVAL_V0
        EVAL_V1
        return Variant(v0.ToBool() || v1.ToBool());
    }
    case cgac::OP_AND:
    {
        EVAL_V0
        EVAL_V1
        return Variant(v0.ToBool() && v1.ToBool());
    }
    case cgac::OP_EQUAL:
    {
        EVAL_V0
        EVAL_V1
        return Variant(abs(v0.ToFloat() - v1.ToFloat()) < std::numeric_limits<double>::epsilon());
    }
    case cgac::OP_UNEQUAL:
    {
        EVAL_V0
        EVAL_V1
        return Variant(abs(v0.ToFloat() - v1.ToFloat()) >= std::numeric_limits<double>::epsilon());
    }
    case cgac::OP_GREAT:
    {
        EVAL_V0
        EVAL_V1
        return Variant(v0.ToFloat() > v1.ToFloat());
    }
    case cgac::OP_LESS:
    {
        EVAL_V0
        EVAL_V1
        return Variant(v0.ToFloat() < v1.ToFloat());
    }
     case cgac::OP_GREAT_EQ:
    {
        EVAL_V0
        EVAL_V1
        return Variant(v0.ToFloat() >= v1.ToFloat());
    }
    case cgac::OP_LESS_EQ:
    {
        EVAL_V0
        EVAL_V1
        return Variant(v0.ToFloat() <= v1.ToFloat());
    }
    case cgac::OP_ADD:
    {
        EVAL_V0
        EVAL_V1
        if (v0.type == VarType::Float || v1.type == VarType::Float) {
            return Variant(v0.ToFloat() + v1.ToFloat());
        } else {
            return Variant();
        }
    }
    case cgac::OP_SUB:
    {
        EVAL_V0
        EVAL_V1
        if (v0.type == VarType::Float || v1.type == VarType::Float) {
            return Variant(v0.ToFloat() - v1.ToFloat());
        } else {
            return Variant();
        }
    }
    case cgac::OP_MUL:
    {
        EVAL_V0
        EVAL_V1
        if (v0.type == VarType::Float || v1.type == VarType::Float) {
            return Variant(v0.ToFloat() * v1.ToFloat());
        } else {
            return Variant();
        }
    }
    case cgac::OP_DIV:
    {
        EVAL_V0
        EVAL_V1
        auto d1 = v1.ToFloat();
        if (abs(d1) < std::numeric_limits<double>::epsilon()) {
            return Variant();
        }
        if (v0.type == VarType::Float || v1.type == VarType::Float) {
            return Variant(v0.ToFloat() / v1.ToFloat());
        } else {
            return Variant();
        }
    }
    case cgac::OP_MOD:
    {
        EVAL_V0
        EVAL_V1
        if (v0.type == VarType::Float && v1.type == VarType::Float) {
            auto mod = static_cast<int>(v0.f) % static_cast<int>(v1.f);
            return Variant(static_cast<float>(mod));
        } else {
            return Variant();
        }
    }
    case cgac::OP_NEG:
    {
        EVAL_V0
        switch (v0.type)
        {
        case VarType::Float:
            return Variant(-v0.f);
        default:
            assert(0);
            return Variant();
        }
    }
    case cgac::OP_NOT:
    {
        EVAL_V0
        return Variant(!v0.ToBool());
    }
    case cgac::OP_ID:
    {
        return Variant(VarType::String, expr->val.p);
    }
    case cgac::OP_CONST:
    {
        int categ = expr->ty->categ;
        if (categ == cgac::FLOAT) {
            return Variant(expr->val.f);
        } else {
            assert(0);
            return Variant();
        }
    }
    case cgac::OP_STR:
    {
        return Variant(VarType::String, expr->val.p);
    }
    default:
        assert(0);
        return Variant();
    }
}

//////////////////////////////////////////////////////////////////////////
// class EvalExpr::Variant
//////////////////////////////////////////////////////////////////////////

EvalExpr::Variant::Variant()
    : type(VarType::Invalid)
{
}

EvalExpr::Variant::Variant(VarType type)
    : type(type)
{
}

EvalExpr::Variant::Variant(float f)
    : type(VarType::Float)
    , f(f)
{
}

EvalExpr::Variant::Variant(VarType type, const void* p)
    : type(type)
    , p(p)
{
}

bool EvalExpr::Variant::ToBool() const
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

float EvalExpr::Variant::ToFloat() const
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