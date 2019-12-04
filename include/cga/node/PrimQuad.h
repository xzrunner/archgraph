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

    virtual void Execute() override;

private:
    void BuildModel();

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "cga/node/PrimQuad.parm.h"
#include <hdiop/node_parms_gen.h>
#undef PARM_FILEPATH

}; // PrimQuad

}
}