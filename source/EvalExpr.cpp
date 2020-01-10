#include "cga/EvalExpr.h"
#include "cga/Variant.h"

#include <cgac/Expression.h>

#define EVAL_V0                       \
auto v0 = Eval(expr->kids[0]);        \
if (!v0) {                            \
    return nullptr;                   \
}

#define EVAL_V1                       \
auto v1 = Eval(expr->kids[1]);        \
if (!v1) {                            \
    return nullptr;                   \
}

namespace cga
{

VarPtr EvalExpr::Eval(const cgac::ExprNodePtr& expr)
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
        return std::make_unique<BoolVar>(v0->ToBool() || v1->ToBool());
    }
    case cgac::OP_AND:
    {
        EVAL_V0
        EVAL_V1
        return std::make_unique<BoolVar>(v0->ToBool() && v1->ToBool());
    }
    case cgac::OP_EQUAL:
    {
        EVAL_V0
        EVAL_V1
        return std::make_unique<BoolVar>(abs(v0->ToFloat() - v1->ToFloat()) < std::numeric_limits<double>::epsilon());
    }
    case cgac::OP_UNEQUAL:
    {
        EVAL_V0
        EVAL_V1
        return std::make_unique<BoolVar>(abs(v0->ToFloat() - v1->ToFloat()) >= std::numeric_limits<double>::epsilon());
    }
    case cgac::OP_GREAT:
    {
        EVAL_V0
        EVAL_V1
        return std::make_unique<BoolVar>(v0->ToFloat() > v1->ToFloat());
    }
    case cgac::OP_LESS:
    {
        EVAL_V0
        EVAL_V1
        return std::make_unique<BoolVar>(v0->ToFloat() < v1->ToFloat());
    }
     case cgac::OP_GREAT_EQ:
    {
        EVAL_V0
        EVAL_V1
        return std::make_unique<BoolVar>(v0->ToFloat() >= v1->ToFloat());
    }
    case cgac::OP_LESS_EQ:
    {
        EVAL_V0
        EVAL_V1
        return std::make_unique<BoolVar>(v0->ToFloat() <= v1->ToFloat());
    }
    case cgac::OP_ADD:
    {
        EVAL_V0
        EVAL_V1
        if (v0->Type() == VarType::Float || v1->Type() == VarType::Float) {
            return std::make_unique<FloatVar>(v0->ToFloat() + v1->ToFloat());
        } else {
            return nullptr;
        }
    }
    case cgac::OP_SUB:
    {
        EVAL_V0
        EVAL_V1
        if (v0->Type() == VarType::Float || v1->Type() == VarType::Float) {
            return std::make_unique<FloatVar>(v0->ToFloat() - v1->ToFloat());
        } else {
            return nullptr;
        }
    }
    case cgac::OP_MUL:
    {
        EVAL_V0
        EVAL_V1
        if (v0->Type() == VarType::Float || v1->Type() == VarType::Float) {
            return std::make_unique<FloatVar>(v0->ToFloat() * v1->ToFloat());
        } else {
            return nullptr;
        }
    }
    case cgac::OP_DIV:
    {
        EVAL_V0
        EVAL_V1
        auto d1 = v1->ToFloat();
        if (abs(d1) < std::numeric_limits<double>::epsilon()) {
            return nullptr;
        }
        if (v0->Type() == VarType::Float || v1->Type() == VarType::Float) {
            return std::make_unique<FloatVar>(v0->ToFloat() / v1->ToFloat());
        } else {
            return nullptr;
        }
    }
    case cgac::OP_MOD:
    {
        EVAL_V0
        EVAL_V1
        if (v0->Type() == VarType::Float && v1->Type() == VarType::Float) {
            auto mod = static_cast<int>(v0->ToFloat()) % static_cast<int>(v1->ToFloat());
            return std::make_unique<FloatVar>(static_cast<float>(mod));
        } else {
            return nullptr;
        }
    }
    case cgac::OP_NEG:
    {
        EVAL_V0
        switch (v0->Type())
        {
        case VarType::Float:
            return std::make_unique<FloatVar>(-v0->ToFloat());
        default:
            assert(0);
            return nullptr;
        }
    }
    case cgac::OP_NOT:
    {
        EVAL_V0
        return std::make_unique<BoolVar>(!v0->ToBool());
    }
    case cgac::OP_MEMBER:
    {
        EVAL_V0

        std::string base = static_cast<const char*>(expr->val.p);
        std::string attr = v0->ToString();
        return std::make_unique<StringVar>(base + attr);
    }
    case cgac::OP_ID:
    {
        return std::make_unique<StringVar>(static_cast<const char*>(expr->val.p));
    }
    case cgac::OP_CONST:
    {
        int categ = expr->ty->categ;
        if (categ == cgac::FLOAT) {
            return std::make_unique<FloatVar>(expr->val.f);
        } else {
            assert(0);
            return nullptr;
        }
    }
    case cgac::OP_STR:
    {
        return std::make_unique<StringVar>(static_cast<const char*>(expr->val.p));
    }
    default:
        assert(0);
        return nullptr;
    }
}

}