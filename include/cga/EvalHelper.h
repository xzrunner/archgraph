#pragma once

#include "cga/VarType.h"
#include "cga/typedef.h"

#include <cgac/typedef.h>

#include <rttr/type.h>

namespace cga
{

struct RelativeFloat;

class EvalHelper
{
public:
    static bool SetPropVal(rttr::property prop, rttr::instance obj,
        const VarPtr& val);

    static VarType ResolveSizeVal(const cgac::ExprNodePtr& expr,
        RelativeFloat& out_flt, std::string& out_str);

}; // EvalHelper

}