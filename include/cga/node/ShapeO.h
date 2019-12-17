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

    virtual void Execute() override;

    virtual std::shared_ptr<Geometry> GetGeo(int idx = -1) const override;

private:
    std::shared_ptr<Geometry> m_rem_geo = nullptr;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "cga/node/ShapeO.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // ShapeO

}
}