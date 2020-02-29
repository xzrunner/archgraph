#pragma once

#include "archgraph/Operation.h"

#include <SM_Vector.h>

namespace archgraph
{
namespace op
{

class Switch : public Operation
{
public:
    Switch()
    {
        m_imports = {
            {{ OpVarType::Any, "in" }},
        };
        m_exports = {
            {{ OpVarType::Any, "else" }},
        };
    }

    virtual void OnParmChanged(const std::string& parm_name) override;

    virtual void Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
        const EvalContext& ctx) override;

    virtual void Setup(const std::vector<cga::ExprNodePtr>& parms,
        const Rule::CompoundSel& selectors, const EvalContext& ctx) override;

private:
    void SetupExports();

private:
    std::vector<cga::ExprNodePtr> m_case_exprs;

    RTTR_ENABLE(Operation)

#define PARM_FILEPATH "archgraph/op/Switch.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Switch

}
}