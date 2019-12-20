#pragma once

#include "cga/Node.h"

#include <SM_Vector.h>

namespace cga
{
namespace node
{

class PrimQuad : public Node
{
public:
    PrimQuad()
    {
        m_exports = {
            {{ NodeVarType::Any, "out" }},
        };
    }

    virtual void Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out) override;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "cga/node/PrimQuad.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // PrimQuad

}
}