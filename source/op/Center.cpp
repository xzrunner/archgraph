#include "cga/op/Center.h"

namespace cga
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