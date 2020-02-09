#pragma once

#include "ce/Operation.h"

#include <SM_Vector.h>

namespace ce
{
namespace op
{

class PrimPoly : public Operation
{
public:
    PrimPoly()
    {
        m_exports = {
            {{ OpVarType::Any, "out" }},
        };
    }

    virtual void Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
        const EvalContext& ctx) override;

    RTTR_ENABLE(Operation)

#define PARM_FILEPATH "ce/op/PrimPoly.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // PrimPoly

}
}