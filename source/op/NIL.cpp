#include "ce/op/NIL.h"

namespace ce
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