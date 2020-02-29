#pragma once

#include "archgraph/Operation.h"

#include <SM_Vector.h>
#include <halfedge/typedef.h>

namespace archgraph
{
namespace op
{

class Split : public Operation
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

        float CalcAbsoluteSize() const;
        float CalcRelativeSize() const;

        bool IsRepeat() const;
        bool IsAbsolute() const;
        bool IsFloating() const;

        bool IsInvalid() const { return size_type == SizeType::None && children.empty(); }
    };

public:
    Split()
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

    virtual void Setup(const std::vector<cga::ExprNodePtr>& parms,
        const Rule::CompoundSel& selectors, const EvalContext& ctx) override;

    void SetupExports();

private:
    void AddAttr(const std::vector<GeoPtr>& geos) const;

    void Execute(const sm::vec3& normal, const GeoPtr& in,
        std::vector<GeoPtr>& out, const EvalContext& ctx);

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
        const std::vector<Part>& parts, bool repeat);
    static std::vector<GeoPtr> CutGeoNoRepeat(float& begin, float end, CutContext& ctx,
        const std::vector<Part>& parts);

    static std::vector<float> CalcPartCutSizes(float begin, float end, const std::vector<Part>& parts);

    static Part SelectorToPart(const Rule::SelPtr& selector, const EvalContext& ctx);

    static he::PolyhedronPtr CutSingle(float pos, CutContext& ctx);

    RTTR_ENABLE(Operation)

#define PARM_FILEPATH "archgraph/op/Split.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Split

}
}