#pragma once

#include "cga/VarType.h"

#include <cgac/typedef.h>

#include <rttr/type.h>

namespace cga
{

struct RelativeFloat;
struct Variant;

class EvalHelper
{
public:
    static bool SetPropVal(rttr::property prop, rttr::instance obj,
        const Variant& val);

    static VarType ResolveSizeVal(const cgac::ExprNodePtr& expr,
        RelativeFloat& out_flt, std::string& out_str);

}; // EvalHelper

}