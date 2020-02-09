#include "ce/op/AlignScopeToGeo.h"

namespace ce
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