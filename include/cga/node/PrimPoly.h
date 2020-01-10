#pragma once

#include "cga/Node.h"

#include <SM_Vector.h>

namespace cga
{
namespace node
{

class PrimPoly : public Node
{
public:
    PrimPoly()
    {
        m_exports = {
            {{ NodeVarType::Any, "out" }},
        };
    }

    virtual void Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
        const EvalContext& ctx) override;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "cga/node/PrimPoly.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // PrimPoly

}
}