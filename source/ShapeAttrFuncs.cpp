#include "cga/ShapeAttrFuncs.h"
#include "cga/Variant.h"
#include "cga/Geometry.h"

#include <assert.h>

namespace cga
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