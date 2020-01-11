#pragma once

#include "cga/Function.h"

namespace cga
{
namespace func
{

// scope attribute

class ScopeSizeX : public Function
{
public:
    virtual VarPtr Eval(const std::vector<VarPtr>& parms,
        const std::vector<GeoPtr>& geos, std::ostream& console) const override;
}; // ScopeSizeX

}
}