#pragma once

#include "cga/VarType.h"


#include <rttr/type.h>

namespace cga
{

struct Variant;

class EvalHelper
{
public:
    static bool SetPropVal(rttr::property prop, rttr::instance obj,
        const Variant& val);


}; // EvalHelper

}