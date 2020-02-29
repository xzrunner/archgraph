#pragma once

#include "archgraph/typedef.h"

#include <cga/typedef.h>

#include <memory>

namespace archgraph
{

class EvalContext;

class EvalExpr
{
public:
    static VarPtr Eval(const cga::ExprNodePtr& expr,
        const EvalContext& ctx, const GeoPtr& geo = nullptr);

    static VarPtr EvalNoExpand(const cga::ExprNodePtr& expr,
        const EvalContext& ctx, const GeoPtr& geo = nullptr);

}; // EvalExpr

}