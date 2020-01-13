#pragma once

#include "cga/Operation.h"

#include <SM_Vector.h>

namespace cga
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

    RTTR_ENABLE(Operation)

#define PARM_FILEPATH "cga/op/PrimQuad.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // PrimQuad

}
}