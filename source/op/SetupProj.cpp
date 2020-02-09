#include "ce/op/SetupProj.h"

namespace ce
{
namespace op
{

void SetupProj::Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out, 
                        const EvalContext& ctx)
{
}

void SetupProj::Setup(const std::vector<cgac::ExprNodePtr>& parms, 
                      const Rule::CompoundSel& selectors, const EvalContext& ctx)
{
}

}
}