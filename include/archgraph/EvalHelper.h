#pragma once

#include "archgraph/VarType.h"
#include "archgraph/typedef.h"

#include <cga/typedef.h>

#include <rttr/type.h>

namespace archgraph
{

struct RelativeFloat;
class EvalContext;

class EvalHelper
{
public:
    static bool SetPropVal(rttr::property prop, rttr::instance obj,
        const VarPtr& val);

    static VarType ResolveSizeVal(const cga::ExprNodePtr& expr,
        const EvalContext& ctx, RelativeFloat& out_flt, std::string& out_str);

}; // EvalHelper

}