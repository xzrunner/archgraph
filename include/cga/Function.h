#pragma once

#include "cga/typedef.h"

#include <vector>

namespace cga
{

class Function
{
public:
    virtual VarPtr Eval(const std::vector<VarPtr>& parms,
        const std::vector<GeoPtr>& geos, std::ostream& console) const = 0;

}; // Function

}