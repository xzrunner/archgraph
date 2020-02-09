#pragma once

#include "ce/VarType.h"
#include "ce/typedef.h"

#include <cgac/typedef.h>

#include <rttr/type.h>

namespace ce
{

struct RelativeFloat;
class EvalContext;

class EvalHelper
{
public:
    static bool SetPropVal(rttr::property prop, rttr::instance obj,
        const VarPtr& val);

    static VarType ResolveSizeVal(const cgac::ExprNodePtr& expr,
        const EvalContext& ctx, RelativeFloat& out_flt, std::string& out_str);

}; // EvalHelper

}