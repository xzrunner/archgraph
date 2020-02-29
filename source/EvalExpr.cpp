#include "archgraph/EvalExpr.h"
#include "archgraph/Variant.h"
#include "archgraph/EvalContext.h"
#include "archgraph/Geometry.h"
#include "archgraph/FuncRegister.h"
#include "archgraph/Function.h"

#include <cga/Expression.h>

#include <sstream>

#define EVAL_V0                          \
auto v0 = Eval(expr->kids[0], ctx, geo); \
if (!v0) {                               \
    return nullptr;                      \
}

#define EVAL_V1                          \
auto v1 = Eval(expr->kids[1], ctx, geo); \
if (!v1) {                               \
    return nullptr;                      \
}

namespace archgraph
{

VarPtr EvalExpr::Eval(const cga::ExprNodePtr& expr,
                      const EvalContext& ctx, const GeoPtr& geo)
{
    auto v = EvalNoExpand(expr, ctx, geo);
    if (v->Type() == VarType::String)
    {
        auto str = check_string(v);

        // eval context
        auto find = ctx.QueryVar(str);
        if (find)
        {
            if (find->expr) {
                return Eval(find->expr, ctx, geo);
            } else {
                switch (find->value.type)
                {
                case dag::VarType::Bool:
                    return std::make_shared<BoolVar>(find->value.b);
                case dag::VarType::Float:
                    return std::make_shared<FloatVar>(find->value.f);
                case dag::VarType::String:
                    return std::make_shared<StringVar>(static_cast<const char*>(find->value.p));
                default:
                    assert(0);
                    return nullptr;
                }
            }
        }

        // geo params
        if (geo) {
            auto var = geo->QueryAttr(str);
            if (var) {
                return var;
            }
        }

        // build-in attr
        auto itr_func = FuncRegister::Instance()->QueryAttrFunc(str);
        if (itr_func) {
            std::stringstream ss;
            return itr_func->Eval(std::vector<VarPtr>(), { geo }, ss);
        }
    }
    return v;
}

VarPtr EvalExpr::EvalNoExpand(const cga::ExprNodePtr& expr,
                              const EvalContext& ctx, const GeoPtr& geo)
{
    switch (expr->op)
    {
    case cga::OP_COMMA:
        return Eval(expr->kids[1], ctx, geo);
    case cga::OP_ASSIGN:
        return Eval(expr->kids[1], ctx, geo);
    case cga::OP_OR:
    {
        EVAL_V0
        EVAL_V1
        return std::make_unique<BoolVar>(check_bool(v0) || check_bool(v1));
    }
    case cga::OP_AND:
    {
        EVAL_V0
        EVAL_V1
        return std::make_unique<BoolVar>(check_bool(v0) && check_bool(v1));
    }
    case cga::OP_EQUAL:
    {
        EVAL_V0
        EVAL_V1
        return std::make_unique<BoolVar>(abs(check_float(v0) - check_float(v1)) < std::numeric_limits<double>::epsilon());
    }
    case cga::OP_UNEQUAL:
    {
        EVAL_V0
        EVAL_V1
        return std::make_unique<BoolVar>(abs(check_float(v0) - check_float(v1)) >= std::numeric_limits<double>::epsilon());
    }
    case cga::OP_GREAT:
    {
        EVAL_V0
        EVAL_V1
        return std::make_unique<BoolVar>(check_float(v0) > check_float(v1));
    }
    case cga::OP_LESS:
    {
        EVAL_V0
        EVAL_V1
        return std::make_unique<BoolVar>(check_float(v0) < check_float(v1));
    }
     case cga::OP_GREAT_EQ:
    {
        EVAL_V0
        EVAL_V1
        return std::make_unique<BoolVar>(check_float(v0) >= check_float(v1));
    }
    case cga::OP_LESS_EQ:
    {
        EVAL_V0
        EVAL_V1
        return std::make_unique<BoolVar>(check_float(v0) <= check_float(v1));
    }
    case cga::OP_ADD:
    {
        EVAL_V0
        EVAL_V1
        if (v0->Type() == VarType::Float || v1->Type() == VarType::Float) {
            return std::make_unique<FloatVar>(check_float(v0) + check_float(v1));
        } else {
            return nullptr;
        }
    }
    case cga::OP_SUB:
    {
        EVAL_V0
        EVAL_V1
        if (v0->Type() == VarType::Float || v1->Type() == VarType::Float) {
            return std::make_unique<FloatVar>(check_float(v0) - check_float(v1));
        } else {
            return nullptr;
        }
    }
    case cga::OP_MUL:
    {
        EVAL_V0
        EVAL_V1
        if (v0->Type() == VarType::Float || v1->Type() == VarType::Float) {
            return std::make_unique<FloatVar>(check_float(v0) * check_float(v1));
        } else {
            return nullptr;
        }
    }
    case cga::OP_DIV:
    {
        EVAL_V0
        EVAL_V1
        auto d1 = check_float(v1);
        if (abs(d1) < std::numeric_limits<double>::epsilon()) {
            return nullptr;
        }
        if (v0->Type() == VarType::Float || v1->Type() == VarType::Float) {
            return std::make_unique<FloatVar>(check_float(v0) / check_float(v1));
        } else {
            return nullptr;
        }
    }
    case cga::OP_MOD:
    {
        EVAL_V0
        EVAL_V1
        if (v0->Type() == VarType::Float && v1->Type() == VarType::Float) {
            auto mod = static_cast<int>(check_float(v0)) % static_cast<int>(check_float(v1));
            return std::make_unique<FloatVar>(static_cast<float>(mod));
        } else {
            return nullptr;
        }
    }
    case cga::OP_NEG:
    {
        EVAL_V0
        switch (v0->Type())
        {
        case VarType::Float:
            return std::make_unique<FloatVar>(-check_float(v0));
        default:
            assert(0);
            return nullptr;
        }
    }
    case cga::OP_NOT:
    {
        EVAL_V0
        return std::make_unique<BoolVar>(!check_bool(v0));
    }
    case cga::OP_MEMBER:
    {
        EVAL_V0
        std::string base = check_string(v0);
        std::string attr = static_cast<const char*>(expr->val.p);
        return std::make_unique<StringVar>(base + "." + attr);
    }
    case cga::OP_ID:
    {
        return std::make_unique<StringVar>(static_cast<const char*>(expr->val.p));
    }
    case cga::OP_CONST:
    {
        int categ = expr->ty->categ;
        if (categ == cga::FLOAT) {
            return std::make_unique<FloatVar>(expr->val.f);
        } else {
            assert(0);
            return nullptr;
        }
    }
    case cga::OP_STR:
    {
        return std::make_unique<StringVar>(static_cast<const char*>(expr->val.p));
    }
    default:
        assert(0);
        return nullptr;
    }
}

}