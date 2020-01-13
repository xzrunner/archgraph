#pragma once

#include "cga/Operation.h"

#include <SM_Vector.h>

namespace cga
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