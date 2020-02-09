#include "ce/op/Center.h"

namespace ce
{
namespace op
{

void Center::Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
                     const EvalContext& ctx)
{
    out.resize(in.size(), nullptr);
}

}
}