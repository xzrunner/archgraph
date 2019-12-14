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
    using loop = std::vector<sm::vec3>;
    using face = std::pair<loop, std::vector<loop>>;
    void OffsetPolygon(std::vector<face>& dst, const std::vector<sm::vec3>& src) const;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "cga/node/Offset.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Offset

}
}