#pragma once

#include "cga/Node.h"
#include "cga/EvalExpr.h"

namespace cga
{
namespace node
{

class SetSize : public Node
{
public:
    struct SizeValue
    {
        float value    = 1.0f;
        bool  relative = true;

        bool operator != (const SizeValue& s) const {
            return value != s.value || relative != s.relative;
        }
    };

public:
    SetSize()
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

    virtual void Setup(const std::vector<cgac::ExprNodePtr>& parms,
        const std::vector<cgac::ExprNodePtr>& selectors, const EvalContext& ctx) override;

private:
    static EvalExpr::VarType ResolveSizeVal(const cgac::ExprNodePtr& expr,
        bool& relative, float& fval, std::string& sval);

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "cga/node/SetSize.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // SetSize

}
}