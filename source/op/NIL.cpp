#include "archgraph/op/NIL.h"

namespace archgraph
{
namespace op
{

void NIL::Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
                    const EvalContext& ctx)
{
    out.resize(in.size(), nullptr);
}

}
}