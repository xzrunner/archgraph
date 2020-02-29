#include "archgraph/op/SetupProj.h"

namespace archgraph
{
namespace op
{

void SetupProj::Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out, 
                        const EvalContext& ctx)
{
}

void SetupProj::Setup(const std::vector<cga::ExprNodePtr>& parms, 
                      const Rule::CompoundSel& selectors, const EvalContext& ctx)
{
}

}
}