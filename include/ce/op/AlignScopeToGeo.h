#pragma once

#include "ce/Operation.h"

#include <SM_Vector.h>

namespace ce
{
namespace op
{

class AlignScopeToGeo : public Operation
{
public:
    AlignScopeToGeo()
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

}; // AlignScopeToGeo

}
}