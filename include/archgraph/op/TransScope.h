#pragma once

#include "archgraph/Operation.h"
#include "archgraph/EvalExpr.h"
#include "archgraph/RelativeFloat.h"

namespace archgraph
{
namespace op
{

class TransScope : public Operation
{
public:
    TransScope()
    {
        m_imports = {
            {{ OpVarType::Any, "in" }},
        };
        m_exports = {
            {{ OpVarType::Any, "out" }},
        };
    }

    virtual void Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
        const EvalContext& ctx) override;

    virtual void Setup(const std::vector<cga::ExprNodePtr>& parms,
        const Rule::CompoundSel& selectors, const EvalContext& ctx) override;

    RTTR_ENABLE(Operation)

#define PARM_FILEPATH "archgraph/op/TransScope.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // TransScope

}
}