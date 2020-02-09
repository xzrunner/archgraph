// creation
#include "cga/op/Extrude.h"
#include "cga/op/Insert.h"
#include "cga/op/PrimCube.h"
#include "cga/op/PrimQuad.h"
#include "cga/op/PrimPoly.h"
// subdivision
#include "cga/op/Comp.h"
#include "cga/op/Offset.h"
#include "cga/op/ShapeO.h"
#include "cga/op/Split.h"
// transformations
#include "cga/op/Center.h"
#include "cga/op/Scale.h"
#include "cga/op/TransScope.h"
// scope
#include "cga/op/AlignScopeToGeo.h"
// flow control
#include "cga/op/NIL.h"
#include "cga/op/Switch.h"
// attributes
#include "cga/op/Color.h"
#include "cga/op/Set.h"

#include <rttr/registration>

#define REGIST_NODE_TYPE(type, name)                           \
	rttr::registration::class_<cga::op::type>("cga::"#name)  \
		.constructor<>()                                       \
	;

#define REGIST_ENUM_ITEM(type, name, label) \
    rttr::value(name, type),                \
    rttr::metadata(type, label)             \

RTTR_REGISTRATION
{

// base

rttr::registration::class_<dag::Node<cga::OpVarType>::Port>("cga::Operation::Port")
	.property("var", &dag::Node<cga::OpVarType>::Port::var)
;

rttr::registration::class_<cga::Operation>("cga::Operation")
	.method("GetImports", &cga::Operation::GetImports)
	.method("GetExports", &cga::Operation::GetExports)
;

rttr::registration::class_<cga::RelativeFloat>("cga::rfloat")
    .property("value",    &cga::RelativeFloat::value)
    .property("relative", &cga::RelativeFloat::relative)
;

// creation
rttr::registration::enumeration<cga::op::Extrude::ExtrusionType>("cga_extrusion_type")
(
    REGIST_ENUM_ITEM(cga::op::Extrude::ExtrusionType::WorldUp,        "world.up",         "World Up"),
    REGIST_ENUM_ITEM(cga::op::Extrude::ExtrusionType::WorldUpFlatTop, "world.up.flatTop", "World Up Flat Top"),
    REGIST_ENUM_ITEM(cga::op::Extrude::ExtrusionType::FaceNormal,     "face.normal",      "Face Normal"),
    REGIST_ENUM_ITEM(cga::op::Extrude::ExtrusionType::VertexNormal,   "vertex.normal",    "Vertex Normal")
);
rttr::registration::class_<cga::op::Extrude>("cga::extrude")
.constructor<>()
#define PARM_FILEPATH "cga/op/Extrude.parm.h"
#define PARM_NODE_CLASS cga::op::Extrude
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::enumeration<cga::op::Insert::UpAxisOfGeo>("cga_insert_up_axis")
(
    REGIST_ENUM_ITEM(cga::op::Insert::UpAxisOfGeo::UpY, "y_up", "Y Up"),
    REGIST_ENUM_ITEM(cga::op::Insert::UpAxisOfGeo::UpZ, "z_up", "Z Up")
);
rttr::registration::enumeration<cga::op::Insert::InsertMode>("cga_insert_mode")
(
    REGIST_ENUM_ITEM(cga::op::Insert::InsertMode::AlignSizeAndPosition,  "align_size_and_position",  "Align Size And Position"),
    REGIST_ENUM_ITEM(cga::op::Insert::InsertMode::KeepSizeAndPosition,   "keep_size_and_position",   "Keep Size And Position"),
    REGIST_ENUM_ITEM(cga::op::Insert::InsertMode::KeepSizeAlignPosition, "keep_size_align_position", "Keep Size Align Position")
);
rttr::registration::class_<cga::op::Insert>("cga::i")
.constructor<>()
#define PARM_FILEPATH "cga/op/Insert.parm.h"
#define PARM_NODE_CLASS cga::op::Insert
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::class_<cga::op::PrimCube>("cga::prim_cube")
.constructor<>()
#define PARM_FILEPATH "cga/op/PrimCube.parm.h"
#define PARM_NODE_CLASS cga::op::PrimCube
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::class_<cga::op::PrimQuad>("cga::prim_quad")
.constructor<>()
#define PARM_FILEPATH "cga/op/PrimQuad.parm.h"
#define PARM_NODE_CLASS cga::op::PrimQuad
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::class_<cga::op::PrimPoly>("cga::prim_poly")
.constructor<>()
#define PARM_FILEPATH "cga/op/PrimPoly.parm.h"
#define PARM_NODE_CLASS cga::op::PrimPoly
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;

// subdivision
rttr::registration::enumeration<cga::op::Comp::Type>("cga_comp_type")
(
    REGIST_ENUM_ITEM(cga::op::Comp::Type::Faces,     "f",  "Faces"),
    REGIST_ENUM_ITEM(cga::op::Comp::Type::Edges,     "e",  "Edges"),
    REGIST_ENUM_ITEM(cga::op::Comp::Type::FaceEdges, "fe", "Face Edges"),
    REGIST_ENUM_ITEM(cga::op::Comp::Type::Vertices,  "v",  "Vertices"),
    REGIST_ENUM_ITEM(cga::op::Comp::Type::Groups,    "g",  "Groups"),
    REGIST_ENUM_ITEM(cga::op::Comp::Type::Materials, "m",  "Materials"),
    REGIST_ENUM_ITEM(cga::op::Comp::Type::Holes,     "h",  "Holes")
);
rttr::registration::enumeration<cga::op::Comp::Selector>("cga_comp_selector")
(
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::Front,  "front",  "Front"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::Back,   "back",   "Back"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::Left,   "left",   "Left"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::Right,  "right",  "Right"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::Top,    "top",    "Top"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::Bottom, "bottom", "Bottom"),

    REGIST_ENUM_ITEM(cga::op::Comp::Selector::ObjFront,  "object.front",  "Object Front"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::ObjBack,   "object.back",   "Object Back"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::ObjLeft,   "object.left",   "Object Left"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::ObjRight,  "object.right",  "Object Right"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::ObjTop,    "object.top",    "Object Top"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::ObjBottom, "object.bottom", "Object Bottom"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::ObjSide,   "object.side",   "Object Side"),

    REGIST_ENUM_ITEM(cga::op::Comp::Selector::WorldSouth, "world.south", "World South"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::WorldNorth, "world.north", "World North"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::WorldWest,  "world.west",  "World West"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::WorldEast,  "world.east",  "World East"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::WorldUp,    "world.up",    "World Up"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::WorldDown,  "world.down",  "World Down"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::WorldSide,  "world.side",  "World Side"),

    REGIST_ENUM_ITEM(cga::op::Comp::Selector::Vertical,   "vertical",   "Vertical"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::Horizontal, "horizontal", "Horizontal"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::Aslant,     "aslant",     "Aslant"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::Nutant,     "nutant",     "Nutant"),

    REGIST_ENUM_ITEM(cga::op::Comp::Selector::Side, "side", "Side"),

    REGIST_ENUM_ITEM(cga::op::Comp::Selector::Border, "border", "Border"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::Inside, "inside", "Inside"),

    REGIST_ENUM_ITEM(cga::op::Comp::Selector::Eave,   "eave",   "Eave"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::Hip,    "hip",    "Hip"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::Valley, "valley", "Valley"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::Ridge,  "ridge",  "Ridge"),

    REGIST_ENUM_ITEM(cga::op::Comp::Selector::StreetFront, "street.front", "Street Front"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::StreetBack,  "street.back",  "Street Back"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::StreetLeft,  "street.left",  "Street Left"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::StreetRight, "street.right", "Street Right"),
    REGIST_ENUM_ITEM(cga::op::Comp::Selector::StreetSide,  "street.side",  "Street Side"),

    REGIST_ENUM_ITEM(cga::op::Comp::Selector::All, "all", "All"),

    REGIST_ENUM_ITEM(cga::op::Comp::Selector::Index, "index", "Index")
);
rttr::registration::class_<cga::op::Comp>("cga::comp")
.constructor<>()
#define PARM_FILEPATH "cga/op/Comp.parm.h"
#define PARM_NODE_CLASS cga::op::Comp
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::enumeration<cga::op::Offset::Selector>("cga_offset_selector")
(
    REGIST_ENUM_ITEM(cga::op::Offset::Selector::All,    "all",    "All"),
    REGIST_ENUM_ITEM(cga::op::Offset::Selector::Inside, "inside", "Inside"),
    REGIST_ENUM_ITEM(cga::op::Offset::Selector::Border, "border", "Border")
);
rttr::registration::class_<cga::op::Offset>("cga::offset")
.constructor<>()
#define PARM_FILEPATH "cga/op/Offset.parm.h"
#define PARM_NODE_CLASS cga::op::Offset
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::class_<cga::op::ShapeO>("cga::shapeo")
.constructor<>()
#define PARM_FILEPATH "cga/op/ShapeO.parm.h"
#define PARM_NODE_CLASS cga::op::ShapeO
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::enumeration<cga::op::Split::Axis>("cga_split_axis")
(
    REGIST_ENUM_ITEM(cga::op::Split::Axis::X, "x", "X"),
    REGIST_ENUM_ITEM(cga::op::Split::Axis::Y, "y", "Y"),
    REGIST_ENUM_ITEM(cga::op::Split::Axis::Z, "z", "Z")
);
rttr::registration::enumeration<cga::op::Split::SizeType>("cga_split_size")
(
    REGIST_ENUM_ITEM(cga::op::Split::SizeType::None,     "none",     "None"),
    REGIST_ENUM_ITEM(cga::op::Split::SizeType::Absolute, "absolute", "Absolute"),
    REGIST_ENUM_ITEM(cga::op::Split::SizeType::Relative, "relative", "Relative"),
    REGIST_ENUM_ITEM(cga::op::Split::SizeType::Floating, "floating", "Floating")
);
rttr::registration::class_<cga::op::Split::Part>("cga::split_part")
    .constructor()(rttr::policy::ctor::as_object)
    .property("type", &cga::op::Split::Part::size_type)
    .property("size",   &cga::op::Split::Part::size)
    .property("repeat", &cga::op::Split::Part::repeat)
;
rttr::registration::class_<cga::op::Split>("cga::split")
.constructor<>()
#define PARM_FILEPATH "cga/op/Split.parm.h"
#define PARM_NODE_CLASS cga::op::Split
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;

// transformations
rttr::registration::class_<cga::op::Center>("cga::center")
.constructor<>()
#define PARM_FILEPATH "cga/op/Center.parm.h"
#define PARM_NODE_CLASS cga::op::Center
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::class_<cga::op::Scale>("cga::s")
.constructor<>()
#define PARM_FILEPATH "cga/op/Scale.parm.h"
#define PARM_NODE_CLASS cga::op::Scale
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::class_<cga::op::TransScope>("cga::t")
.constructor<>()
#define PARM_FILEPATH "cga/op/TransScope.parm.h"
#define PARM_NODE_CLASS cga::op::TransScope
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;

// scope
rttr::registration::class_<cga::op::AlignScopeToGeo>("cga::alignScopeToGeometry")
.constructor<>()
#define PARM_FILEPATH "cga/op/AlignScopeToGeo.parm.h"
#define PARM_NODE_CLASS cga::op::AlignScopeToGeo
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;

// flow control
rttr::registration::class_<cga::op::NIL>("cga::NIL")
.constructor<>()
;
rttr::registration::class_<cga::op::Switch>("cga::switch")
.constructor<>()
#define PARM_FILEPATH "cga/op/Switch.parm.h"
#define PARM_NODE_CLASS cga::op::Switch
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;

// attributes
rttr::registration::class_<cga::op::Color>("cga::color")
.constructor<>()
#define PARM_FILEPATH "cga/op/Color.parm.h"
#define PARM_NODE_CLASS cga::op::Color
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::class_<cga::op::Set>("cga::set")
.constructor<>()
#define PARM_FILEPATH "cga/op/Set.parm.h"
#define PARM_NODE_CLASS cga::op::Set
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