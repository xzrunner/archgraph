#pragma once

#include "cga/Operation.h"

#include <SM_Vector.h>

namespace cga
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

    virtual void Setup(const std::vector<cgac::ExprNodePtr>& parms,
        const Rule::CompoundSel& selectors, const EvalContext& ctx) override;

private:
    void SetupExports();

private:
    std::vector<cgac::ExprNodePtr> m_case_exprs;

    RTTR_ENABLE(Operation)

#define PARM_FILEPATH "cga/op/Switch.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Switch

}
}