#pragma once

#include "cga/Node.h"

#include <SM_Vector.h>

namespace cga
{
namespace node
{

class Color : public Node
{
public:
    Color()
    {
        m_imports = {
            {{ NodeVarType::Any, "in" }},
        };
    }

    virtual void Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
        const EvalContext& ctx) override;

    virtual void Setup(const std::vector<cgac::ExprNodePtr>& parms,
        const Rule::CompoundSel& selectors, const EvalContext& ctx) override;

private:
    static sm::vec3 ExprToColor(const EvalContext& ctx, const cgac::ExprNodePtr& expr);
    static sm::vec3 StringToColor(const std::string& str);

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "cga/node/Color.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Color

}
}