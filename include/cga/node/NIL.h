#pragma once

#include "cga/Node.h"

#include <SM_Vector.h>

namespace cga
{
namespace node
{

class NIL : public Node
{
public:
    NIL()
    {
        m_imports = {
            {{ NodeVarType::Any, "in" }},
        };
        m_exports = {
            {{ NodeVarType::Any, "out" }},
        };
    }

    virtual void Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
        const EvalContext& ctx) override;

    RTTR_ENABLE(Node)

}; // NIL

}
}