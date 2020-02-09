#pragma once

#include "ce/Operation.h"
#include "ce/EvalExpr.h"
#include "ce/RelativeFloat.h"

namespace ce
{
namespace op
{

class Scale : public Operation
{
public:
    Scale()
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

    virtual void Setup(const std::vector<cgac::ExprNodePtr>& parms,
        const Rule::CompoundSel& selectors, const EvalContext& ctx) override;

    RTTR_ENABLE(Operation)

#define PARM_FILEPATH "ce/op/Scale.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Scale

}
}