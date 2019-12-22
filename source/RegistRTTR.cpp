// creation
#include "cga/node/Extrude.h"
#include "cga/node/PrimCube.h"
#include "cga/node/PrimQuad.h"
// subdivision
#include "cga/node/Comp.h"
#include "cga/node/Offset.h"
#include "cga/node/ShapeO.h"
// attributes
#include "cga/node/Color.h"

#include <rttr/registration>

#define REGIST_NODE_TYPE(type, name)                           \
	rttr::registration::class_<cga::node::type>("cga::"#name)  \
		.constructor<>()                                       \
	;

#define REGIST_ENUM_ITEM(type, name, label) \
    rttr::value(name, type),                \
    rttr::metadata(type, label)             \

RTTR_REGISTRATION
{

// base

rttr::registration::class_<dag::Node<cga::NodeVarType>::Port>("cga::Node::Port")
	.property("var", &dag::Node<cga::NodeVarType>::Port::var)
;

rttr::registration::class_<cga::Node>("cga::Node")
	.method("GetImports", &cga::Node::GetImports)
	.method("GetExports", &cga::Node::GetExports)
;

// creation
rttr::registration::enumeration<cga::node::Extrude::ExtrusionType>("cga_extrusion_type")
(
    REGIST_ENUM_ITEM(cga::node::Extrude::ExtrusionType::WorldUp,        "world_up",          "World Up"),
    REGIST_ENUM_ITEM(cga::node::Extrude::ExtrusionType::WorldUpFlatTop, "world_up_flat_top", "World Up Flat Top"),
    REGIST_ENUM_ITEM(cga::node::Extrude::ExtrusionType::FaceNormal,     "face_normal",       "Face Normal"),
    REGIST_ENUM_ITEM(cga::node::Extrude::ExtrusionType::VertexNormal,   "vertex_normal",     "Vertex Normal")
);
rttr::registration::class_<cga::node::Extrude>("cga::extrude")
.constructor<>()
#define PARM_FILEPATH "cga/node/Extrude.parm.h"
#define PARM_NODE_CLASS cga::node::Extrude
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::class_<cga::node::PrimCube>("cga::prim_cube")
.constructor<>()
#define PARM_FILEPATH "cga/node/PrimCube.parm.h"
#define PARM_NODE_CLASS cga::node::PrimCube
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::class_<cga::node::PrimQuad>("cga::prim_quad")
.constructor<>()
#define PARM_FILEPATH "cga/node/PrimQuad.parm.h"
#define PARM_NODE_CLASS cga::node::PrimQuad
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;

// subdivision
rttr::registration::enumeration<cga::node::Comp::Type>("cga_comp_type")
(
    REGIST_ENUM_ITEM(cga::node::Comp::Type::Faces,     "f",  "Faces"),
    REGIST_ENUM_ITEM(cga::node::Comp::Type::Edges,     "e",  "Edges"),
    REGIST_ENUM_ITEM(cga::node::Comp::Type::FaceEdges, "fe", "Face Edges"),
    REGIST_ENUM_ITEM(cga::node::Comp::Type::Vertices,  "v",  "Vertices"),
    REGIST_ENUM_ITEM(cga::node::Comp::Type::Groups,    "g",  "Groups"),
    REGIST_ENUM_ITEM(cga::node::Comp::Type::Materials, "m",  "Materials"),
    REGIST_ENUM_ITEM(cga::node::Comp::Type::Holes,     "h",  "Holes")
);
rttr::registration::enumeration<cga::node::Comp::Selector>("cga_comp_selector")
(
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::Front,  "front",  "Front"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::Back,   "back",   "Back"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::Left,   "left",   "Left"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::Right,  "right",  "Right"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::Top,    "top",    "Top"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::Bottom, "bottom", "Bottom"),

    REGIST_ENUM_ITEM(cga::node::Comp::Selector::ObjFront,  "object.front",  "Object Front"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::ObjBack,   "object.back",   "Object Back"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::ObjLeft,   "object.left",   "Object Left"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::ObjRight,  "object.right",  "Object Right"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::ObjTop,    "object.top",    "Object Top"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::ObjBottom, "object.bottom", "Object Bottom"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::ObjSide,   "object.side",   "Object Side"),

    REGIST_ENUM_ITEM(cga::node::Comp::Selector::WorldSouth, "world.south", "World South"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::WorldNorth, "world.north", "World North"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::WorldWest,  "world.west",  "World West"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::WorldEast,  "world.east",  "World East"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::WorldUp,    "world.up",    "World Up"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::WorldDown,  "world.down",  "World Down"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::WorldSide,  "world.side",  "World Side"),

    REGIST_ENUM_ITEM(cga::node::Comp::Selector::Vertical,   "vertical",   "Vertical"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::Horizontal, "horizontal", "Horizontal"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::Aslant,     "aslant",     "Aslant"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::Nutant,     "nutant",     "Nutant"),

    REGIST_ENUM_ITEM(cga::node::Comp::Selector::Side, "side", "Side"),

    REGIST_ENUM_ITEM(cga::node::Comp::Selector::Border, "border", "Border"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::Inside, "inside", "Inside"),

    REGIST_ENUM_ITEM(cga::node::Comp::Selector::Eave,   "eave",   "Eave"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::Hip,    "hip",    "Hip"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::Valley, "valley", "Valley"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::Ridge,  "ridge",  "Ridge"),

    REGIST_ENUM_ITEM(cga::node::Comp::Selector::StreetFront, "street.front", "Street Front"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::StreetBack,  "street.back",  "Street Back"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::StreetLeft,  "street.left",  "Street Left"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::StreetRight, "street.right", "Street Right"),
    REGIST_ENUM_ITEM(cga::node::Comp::Selector::StreetSide,  "street.side",  "Street Side"),

    REGIST_ENUM_ITEM(cga::node::Comp::Selector::All, "all", "All"),

    REGIST_ENUM_ITEM(cga::node::Comp::Selector::Index, "index", "Index")
);
rttr::registration::class_<cga::node::Comp>("cga::comp")
.constructor<>()
#define PARM_FILEPATH "cga/node/Comp.parm.h"
#define PARM_NODE_CLASS cga::node::Comp
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::enumeration<cga::node::Offset::Selector>("cga_offset_selector")
(
    REGIST_ENUM_ITEM(cga::node::Offset::Selector::All,    "all",    "All"),
    REGIST_ENUM_ITEM(cga::node::Offset::Selector::Inside, "inside", "Inside"),
    REGIST_ENUM_ITEM(cga::node::Offset::Selector::Border, "border", "Border")
);
rttr::registration::class_<cga::node::Offset>("cga::offset")
.constructor<>()
#define PARM_FILEPATH "cga/node/Offset.parm.h"
#define PARM_NODE_CLASS cga::node::Offset
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::class_<cga::node::ShapeO>("cga::shapeo")
.constructor<>()
#define PARM_FILEPATH "cga/node/ShapeO.parm.h"
#define PARM_NODE_CLASS cga::node::ShapeO
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;

// attributes
rttr::registration::class_<cga::node::Color>("cga::color")
.constructor<>()
#define PARM_FILEPATH "cga/node/Color.parm.h"
#define PARM_NODE_CLASS cga::node::Color
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;

}

namespace cga
{

void regist_rttr()
{
}

}