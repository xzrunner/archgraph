#pragma once

#include "archgraph/Operation.h"

#include <SM_Vector.h>

namespace archgraph
{
namespace op
{

class PrimQuad : public Operation
{
public:
    PrimQuad()
    {
        m_exports = {
            {{ OpVarType::Any, "out" }},
        };
    }

    virtual void Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
        const EvalContext& ctx) override;

    virtual void Setup(const std::vector<cga::ExprNodePtr>& parms,
        const Rule::CompoundSel& selectors, const EvalContext& ctx) override;

    RTTR_ENABLE(Operation)

#define PARM_FILEPATH "archgraph/op/PrimQuad.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // PrimQuad

}
}