#pragma once

#include "cga/typedef.h"

#include <cgac/typedef.h>

#include <memory>

namespace cga
{

class EvalContext;

class EvalExpr
{
public:
    static VarPtr Eval(const cgac::ExprNodePtr& expr, const EvalContext& ctx);

private:
    static VarPtr EvalNoRecursion(const cgac::ExprNodePtr& expr, const EvalContext& ctx);

}; // EvalExpr

}