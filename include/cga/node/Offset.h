#pragma once

#include "cga/Node.h"

#include <SM_Vector.h>

namespace cga
{
namespace node
{

class Offset : public Node
{
public:
    enum class Selector
    {
        All,
        Inside,
        Border,
    };

public:
    Offset()
    {
        m_imports = {
            {{ NodeVarType::Any, "in" }},
        };
        m_exports = {
            {{ NodeVarType::Any, "out" }},
        };
    }

    virtual void Execute() override;

private:
    std::vector<std::vector<sm::vec3>>
        OffsetPolygon(const std::vector<sm::vec3>& poly) const;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "cga/node/Offset.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Offset

}
}