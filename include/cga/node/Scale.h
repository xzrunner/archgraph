#pragma once

#include "cga/Node.h"
#include "cga/EvalExpr.h"
#include "cga/RelativeFloat.h"

namespace cga
{
namespace node
{

class Scale : public Node
{
public:
    Scale()
    {
        m_imports = {
            {{ NodeVarType::Any, "in" }},
        };
        m_exports = {
            {{ NodeVarType::Any, "out" }},
        };
    }

    virtual void Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
        const EvalContext& ctx) override;

    virtual void Setup(const std::vector<cgac::ExprNodePtr>& parms,
        const Rule::CompoundSel& selectors, const EvalContext& ctx) override;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "cga/node/Scale.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Scale

}
}