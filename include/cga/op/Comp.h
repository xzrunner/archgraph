#pragma once

#include "cga/Operation.h"

namespace cga
{
namespace op
{

class Comp : public Operation
{
public:
    enum class Type
    {
        Faces,
        Edges,
        FaceEdges,
        Vertices,
        Groups,
        Materials,
        Holes,
    };

    enum class Selector
    {
        // The y-normals of the components are analyzed by classifying their directions into the corresponding quadrants (in relation to the local coordinate system of the current shape).
        Front,
        Back,
        Left,
        Right,
        Top,
        Bottom,

        // The y-normals of the components are analyzed by classifying their directions into the corresponding quadrants (in relation to the object coordinate system of the current initial shape).
        ObjFront,
        ObjBack,
        ObjLeft,
        ObjRight,
        ObjTop,
        ObjBottom,
        ObjSide,

        // The y-normals of the components are analyzed by classifying their directions into the corresponding quadrants (in relation to the world coordinate system).
        WorldSouth,
        WorldNorth,
        WorldWest,
        WorldEast,
        WorldUp,
        WorldDown,
        WorldSide,

        // The y-normals are analyzed in relation to the xz-plane of the current shape's local coordinate system. The angle between normals and xz-plane is used to classify the components with the ranges in degrees: horizontal: ]78.75, 90] aslant: ]11.25, 78.75] vertical: ] - 11.25, 11.25] nutant: ] - 78.55, -11.25] horizontal: ] - 90, -78.75]
        Vertical,
        Horizontal,
        Aslant,
        Nutant,

        // selects all but the horizontal components.
        Side,

        // components at the border of or fully inside the geometry respectively. Border edges are connected to only one face; border faces contain one or more border edges; border vertices are start or end point of one or more border edges. This selector does not work on holes.
        Border,
        Inside,

        // these selectors work on edges only and are designed to be used in conjunction with roofs. See roof edges for more details.
        Eave,
        Hip,
        Valley,
        Ridge,

        // if the streetWidth attribute is available on the initial shape, these selectors can be used to identify street-facing components. See street selectors for more details.
        StreetFront,
        StreetBack,
        StreetLeft,
        StreetRight,
        StreetSide,

        // selects all components.
        All,

        // selects the index-th component (0-based).
        Index,
    };

public:
    Comp()
    {
        m_imports = {
            {{ OpVarType::Any, "in" }},
        };
        SetupExports();
    }

    virtual void Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
        const EvalContext& ctx) override;

    virtual void Setup(const std::vector<cgac::ExprNodePtr>& parms,
        const Rule::CompoundSel& selectors, const EvalContext& ctx) override;

    void SetupExports();

private:
    static GeoPtr BuildComp(const GeoPtr& geo, Selector sel);

    static GeoPtr BuildFace6(const GeoPtr& geo, Selector sel);
    static GeoPtr BuildBorderInside(const GeoPtr& geo, Selector sel);
    static GeoPtr BuildNormalY(const GeoPtr& geo, Selector sel);

    RTTR_ENABLE(Operation)

#define PARM_FILEPATH "cga/op/Comp.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Comp

}
}