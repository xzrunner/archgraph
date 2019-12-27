#pragma once

#include "cga/EvalExpr.h"

#include <rttr/type.h>

namespace cga
{

class EvalHelper
{
public:
    static bool SetPropVal(rttr::property prop, rttr::instance obj,
        const EvalExpr::Variant& val);

}; // EvalHelper

}