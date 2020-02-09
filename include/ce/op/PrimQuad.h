#pragma once

#include "ce/Operation.h"

#include <SM_Vector.h>

namespace ce
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

    virtual void Setup(const std::vector<cgac::ExprNodePtr>& parms,
        const Rule::CompoundSel& selectors, const EvalContext& ctx) override;

    RTTR_ENABLE(Operation)

#define PARM_FILEPATH "ce/op/PrimQuad.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // PrimQuad

}
}