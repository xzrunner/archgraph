#pragma once

#include "ce/Operation.h"

#include <SM_Vector.h>

namespace ce
{
namespace op
{

class Color : public Operation
{
public:
    Color()
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

private:
    static sm::vec3 StringToColor(const std::string& str);

    RTTR_ENABLE(Operation)

#define PARM_FILEPATH "ce/op/Color.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Color

}
}