#include "cga/node/NIL.h"

namespace cga
{
namespace node
{

void NIL::Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
                    const EvalContext& ctx)
{
    out.resize(in.size(), nullptr);
}

}
}