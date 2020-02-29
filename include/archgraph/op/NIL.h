#pragma once

#include "archgraph/Operation.h"

#include <SM_Vector.h>

namespace archgraph
{
namespace op
{

class NIL : public Operation
{
public:
    NIL()
    {
        m_imports = {
            {{ OpVarType::Any, "in" }},
        };
        m_exports = {
            {{ OpVarType::Any, "out" }},
        };
    }

    virtual void Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
        const EvalContext& ctx) override;

    RTTR_ENABLE(Operation)

}; // NIL

}
}