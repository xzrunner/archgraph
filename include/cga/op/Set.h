#pragma once

#include "cga/Operation.h"

namespace cga
{
namespace op
{

class Set : public Operation
{
public:
    Set()
    {
        m_imports = {
            {{ OpVarType::Any, "in" }},
        };
        m_exports = {
            {{ OpVarType::Any, "out" }},
        };
    }

    virtual void OnParmChanged(const std::string& parm_name) override;

    virtual void Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
        const EvalContext& ctx) override;

    virtual void Setup(const std::vector<cgac::ExprNodePtr>& parms,
        const Rule::CompoundSel& selectors, const EvalContext& ctx) override;

private:
    cgac::ExprNodePtr m_val_expr = nullptr;

    RTTR_ENABLE(Operation)

#define PARM_FILEPATH "cga/op/Set.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Set

}
}