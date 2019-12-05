#pragma once

#include "cga/Node.h"

namespace cga
{
namespace node
{

class Extrude : public Node
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
            {{ NodeVarType::Any, "in" }},
        };
        m_exports = {
            {{ NodeVarType::Any, "out" }},
        };
    }

    virtual void Execute() override;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "cga/node/Extrude.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Extrude

}
}