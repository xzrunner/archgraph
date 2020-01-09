#pragma once

#include "cga/Node.h"

#include <SM_Vector.h>
#include <halfedge/typedef.h>

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

    enum class SizeType
    {
        None,

        Absolute,
        Relative,
        Floating,
    };

    struct Part
    {
        SizeType size_type = SizeType::None;
        float    size      = 0;
        bool     repeat    = false;

        std::vector<Part> children;

        Part() {}
        Part(SizeType size_type, float size, bool repeat = false);

        bool operator == (const Part& p) const;

        // return size_val + is_absolute
        std::pair<float, bool> CalcSize() const;

        bool HasFloat() const;
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

    virtual void Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
        const EvalContext& ctx) override;

    virtual void Setup(const std::vector<cgac::ExprNodePtr>& parms,
        const Rule::CompoundSel& selectors, const EvalContext& ctx) override;

    void SetupExports();

private:
    struct CutContext
    {
        CutContext(const sm::vec3& normal, const he::PolyhedronPtr& poly)
            : normal(normal), curr_poly(poly) {}

        float last_pos = -1;

        const sm::vec3 normal;

        he::PolyhedronPtr curr_poly = nullptr;
    };

private:
    static std::vector<GeoPtr> CutGeo(float begin, float end, CutContext& ctx,
        const std::vector<Part>& parts, bool duplicate);
    static std::vector<GeoPtr> CutGeoNoRepeat(float& begin, float end, CutContext& ctx,
        const std::vector<Part>& parts);

    static Part SelectorToPart(const Rule::SelPtr& selector);

    static he::PolyhedronPtr CutSingle(float pos, CutContext& ctx);

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "cga/node/Split.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Split

}
}