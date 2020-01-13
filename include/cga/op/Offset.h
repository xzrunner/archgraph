#pragma once

#include "cga/Operation.h"

#include <SM_Vector.h>

namespace cga
{
namespace op
{

class Offset : public Operation
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
            {{ OpVarType::Any, "in" }},
        };
        m_exports = {
            {{ OpVarType::Any, "out" }},
        };
    }

    virtual void Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
        const EvalContext& ctx) override;

    virtual void Setup(const std::vector<cgac::ExprNodePtr>& parms,
        const Rule::CompoundSel& selectors, const EvalContext& ctx) override;

    RTTR_ENABLE(Operation)

#define PARM_FILEPATH "cga/op/Offset.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Offset

}
}