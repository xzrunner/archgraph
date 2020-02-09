#include "ce/ShapeAttrFuncs.h"
#include "ce/Variant.h"
#include "ce/Geometry.h"

#include <assert.h>

namespace ce
{
namespace func
{

VarPtr ScopeSizeX::Eval(const std::vector<VarPtr>& parms,
                        const std::vector<GeoPtr>& geos, std::ostream& console) const
{
    assert(geos.size() == 1);
    auto& aabb = geos[0]->GetPoly()->GetTopoPoly()->GetAABB();
    return std::make_unique<FloatVar>(aabb.Width());
}

}
}