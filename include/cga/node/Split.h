#pragma once

#include "cga/Node.h"

namespace cga
{
namespace node
{

class Split : public Node
{
public:
    enum class Axis
    {
        X,
        Y,
        Z
    };

public:
    Split()
    {
        m_imports = {
            {{ NodeVarType::Any, "in" }},
        };
        m_exports = {
            {{ NodeVarType::Any, "out" }},
        };
    }

    virtual void Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out) override;

public:
    enum class SizeType
    {
        Absolute,
        Relative,
        Floating,
    };

    struct Part
    {
        SizeType type   = SizeType::Absolute;
        float    size   = 0;
        bool     repeat = false;

        Part(SizeType type, float size, bool repeat = false)
            : type(type), size(size), repeat(repeat)
        {
        }

        bool operator == (const Part& p) const {
            return type == p.type
                && size == p.size
                && repeat == p.repeat;
        }
    };

private:
    std::vector<float> CalcKnifePos(const GeoPtr& geo) const;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "cga/node/Split.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Split

}
}