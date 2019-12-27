#pragma once

#include "cga/Node.h"

#include <SM_Vector.h>

namespace cga
{
namespace node
{

class ShapeO : public Node
{
public:
    enum OutputID
    {
        OUT_SHAPE = 0,
        OUT_REMAINDER,
    };

public:
    ShapeO()
    {
        m_imports = {
            {{ NodeVarType::Any, "in" }},
        };
        m_exports = {
            {{ NodeVarType::Any, "shape" }},
            {{ NodeVarType::Any, "remainder" }},
        };
    }

    virtual void Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
        const EvalContext& ctx) override;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "cga/node/ShapeO.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // ShapeO

}
}