#pragma once

#include "ce/typedef.h"

#include <vector>

namespace ce
{

class Function
{
public:
    virtual VarPtr Eval(const std::vector<VarPtr>& parms,
        const std::vector<GeoPtr>& geos, std::ostream& console) const = 0;

}; // Function

}