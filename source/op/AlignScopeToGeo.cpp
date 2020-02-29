#include "archgraph/op/AlignScopeToGeo.h"

namespace archgraph
{
namespace op
{

void AlignScopeToGeo::Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
                              const EvalContext& ctx)
{
    out.resize(in.size(), nullptr);
}

}
}