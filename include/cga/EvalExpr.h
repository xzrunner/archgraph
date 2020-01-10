#pragma once

#include "cga/typedef.h"

#include <cgac/typedef.h>

#include <memory>

namespace cga
{

class EvalExpr
{
public:
    static VarPtr Eval(const cgac::ExprNodePtr& expr);

}; // EvalExpr

}