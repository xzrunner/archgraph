#pragma once

namespace ce
{

struct RelativeFloat
{
    float value    = 0;
    bool  relative = true;

    RelativeFloat() {}
    RelativeFloat(float value, bool relative = true)
        : value(value), relative(relative) {}

    bool operator != (const RelativeFloat& s) const {
        return value != s.value || relative != s.relative;
    }
};

}