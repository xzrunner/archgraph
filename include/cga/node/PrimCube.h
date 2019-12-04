#pragma once

#include "cga/Node.h"

namespace cga
{
namespace node
{

class PrimCube : public Node
{
public:
    PrimCube()
    {
        m_exports = {
            {{ NodeVarType::Any, "out" }},
        };
    }

    virtual void Execute() override;

private:
    void BuildModel();

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "cga/node/PrimCube.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // PrimCube

}
}