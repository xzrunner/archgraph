#pragma once

#include "archgraph/Operation.h"

namespace archgraph
{
namespace op
{

class Extrude : public Operation
{
public:
    enum class ExtrusionType
    {
        WorldUp,
        WorldUpFlatTop,
        FaceNormal,
        VertexNormal,
    };

public:
    Extrude()
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

#define PARM_FILEPATH "archgraph/op/Extrude.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Extrude

}
}