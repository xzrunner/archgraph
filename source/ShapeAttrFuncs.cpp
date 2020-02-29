#include "archgraph/ShapeAttrFuncs.h"
#include "archgraph/Variant.h"
#include "archgraph/Geometry.h"

#include <assert.h>

namespace archgraph
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