#pragma once

#include "ce/typedef.h"

#include <cgac/typedef.h>

#include <memory>

namespace ce
{

class EvalContext;

class EvalExpr
{
public:
    static VarPtr Eval(const cgac::ExprNodePtr& expr,
        const EvalContext& ctx, const GeoPtr& geo = nullptr);

    static VarPtr EvalNoExpand(const cgac::ExprNodePtr& expr,
        const EvalContext& ctx, const GeoPtr& geo = nullptr);

}; // EvalExpr

}