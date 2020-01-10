#pragma once

#include "cga/Variant.h"

#include <cgac/typedef.h>

#include <memory>

namespace cga
{

class EvalExpr
{
public:
    static Variant Eval(const cgac::ExprNodePtr& expr);

}; // EvalExpr

}