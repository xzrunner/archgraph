#pragma once

#include "cga/Operation.h"

#include <SM_Vector.h>

namespace cga
{
namespace op
{

class ShapeO : public Operation
{
public:
    enum OutputID
    {
        OUT_SHAPE = 0,
        OUT_REMAINDER,
    };

public:
    ShapeO()
    {
        m_imports = {
            {{ OpVarType::Any, "in" }},
        };
        m_exports = {
            {{ OpVarType::Any, "shape" }},
            {{ OpVarType::Any, "remainder" }},
        };
    }

    virtual void Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
        const EvalContext& ctx) override;

    RTTR_ENABLE(Operation)

#define PARM_FILEPATH "cga/op/ShapeO.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // ShapeO

}
}