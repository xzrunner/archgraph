// creation
#include "ce/op/Extrude.h"
#include "ce/op/Insert.h"
#include "ce/op/PrimCube.h"
#include "ce/op/PrimQuad.h"
#include "ce/op/PrimPoly.h"
// subdivision
#include "ce/op/Comp.h"
#include "ce/op/Offset.h"
#include "ce/op/ShapeO.h"
#include "ce/op/Split.h"
// transformations
#include "ce/op/Center.h"
#include "ce/op/Scale.h"
#include "ce/op/TransScope.h"
// scope
#include "ce/op/AlignScopeToGeo.h"
// flow control
#include "ce/op/NIL.h"
#include "ce/op/Switch.h"
// attributes
#include "ce/op/Color.h"
#include "ce/op/Set.h"

#include <rttr/registration>

#define REGIST_NODE_TYPE(type, name)                           \
	rttr::registration::class_<ce::op::type>("ce::"#name)  \
		.constructor<>()                                       \
	;

#define REGIST_ENUM_ITEM(type, name, label) \
    rttr::value(name, type),                \
    rttr::metadata(type, label)             \

RTTR_REGISTRATION
{

// base

rttr::registration::class_<dag::Node<ce::OpVarType>::Port>("ce::Operation::Port")
	.property("var", &dag::Node<ce::OpVarType>::Port::var)
;

rttr::registration::class_<ce::Operation>("ce::Operation")
	.method("GetImports", &ce::Operation::GetImports)
	.method("GetExports", &ce::Operation::GetExports)
;

rttr::registration::class_<ce::RelativeFloat>("ce::rfloat")
    .property("value",    &ce::RelativeFloat::value)
    .property("relative", &ce::RelativeFloat::relative)
;

// creation
rttr::registration::enumeration<ce::op::Extrude::ExtrusionType>("cga_extrusion_type")
(
    REGIST_ENUM_ITEM(ce::op::Extrude::ExtrusionType::WorldUp,        "world.up",         "World Up"),
    REGIST_ENUM_ITEM(ce::op::Extrude::ExtrusionType::WorldUpFlatTop, "world.up.flatTop", "World Up Flat Top"),
    REGIST_ENUM_ITEM(ce::op::Extrude::ExtrusionType::FaceNormal,     "face.normal",      "Face Normal"),
    REGIST_ENUM_ITEM(ce::op::Extrude::ExtrusionType::VertexNormal,   "vertex.normal",    "Vertex Normal")
);
rttr::registration::class_<ce::op::Extrude>("ce::extrude")
.constructor<>()
#define PARM_FILEPATH "ce/op/Extrude.parm.h"
#define PARM_NODE_CLASS ce::op::Extrude
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::enumeration<ce::op::Insert::UpAxisOfGeo>("cga_insert_up_axis")
(
    REGIST_ENUM_ITEM(ce::op::Insert::UpAxisOfGeo::UpY, "y_up", "Y Up"),
    REGIST_ENUM_ITEM(ce::op::Insert::UpAxisOfGeo::UpZ, "z_up", "Z Up")
);
rttr::registration::enumeration<ce::op::Insert::InsertMode>("cga_insert_mode")
(
    REGIST_ENUM_ITEM(ce::op::Insert::InsertMode::AlignSizeAndPosition,  "align_size_and_position",  "Align Size And Position"),
    REGIST_ENUM_ITEM(ce::op::Insert::InsertMode::KeepSizeAndPosition,   "keep_size_and_position",   "Keep Size And Position"),
    REGIST_ENUM_ITEM(ce::op::Insert::InsertMode::KeepSizeAlignPosition, "keep_size_align_position", "Keep Size Align Position")
);
rttr::registration::class_<ce::op::Insert>("ce::i")
.constructor<>()
#define PARM_FILEPATH "ce/op/Insert.parm.h"
#define PARM_NODE_CLASS ce::op::Insert
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::class_<ce::op::PrimCube>("ce::prim_cube")
.constructor<>()
#define PARM_FILEPATH "ce/op/PrimCube.parm.h"
#define PARM_NODE_CLASS ce::op::PrimCube
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::class_<ce::op::PrimQuad>("ce::prim_quad")
.constructor<>()
#define PARM_FILEPATH "ce/op/PrimQuad.parm.h"
#define PARM_NODE_CLASS ce::op::PrimQuad
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::class_<ce::op::PrimPoly>("ce::prim_poly")
.constructor<>()
#define PARM_FILEPATH "ce/op/PrimPoly.parm.h"
#define PARM_NODE_CLASS ce::op::PrimPoly
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;

// subdivision
rttr::registration::enumeration<ce::op::Comp::Type>("cga_comp_type")
(
    REGIST_ENUM_ITEM(ce::op::Comp::Type::Faces,     "f",  "Faces"),
    REGIST_ENUM_ITEM(ce::op::Comp::Type::Edges,     "e",  "Edges"),
    REGIST_ENUM_ITEM(ce::op::Comp::Type::FaceEdges, "fe", "Face Edges"),
    REGIST_ENUM_ITEM(ce::op::Comp::Type::Vertices,  "v",  "Vertices"),
    REGIST_ENUM_ITEM(ce::op::Comp::Type::Groups,    "g",  "Groups"),
    REGIST_ENUM_ITEM(ce::op::Comp::Type::Materials, "m",  "Materials"),
    REGIST_ENUM_ITEM(ce::op::Comp::Type::Holes,     "h",  "Holes")
);
rttr::registration::enumeration<ce::op::Comp::Selector>("cga_comp_selector")
(
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::Front,  "front",  "Front"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::Back,   "back",   "Back"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::Left,   "left",   "Left"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::Right,  "right",  "Right"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::Top,    "top",    "Top"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::Bottom, "bottom", "Bottom"),

    REGIST_ENUM_ITEM(ce::op::Comp::Selector::ObjFront,  "object.front",  "Object Front"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::ObjBack,   "object.back",   "Object Back"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::ObjLeft,   "object.left",   "Object Left"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::ObjRight,  "object.right",  "Object Right"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::ObjTop,    "object.top",    "Object Top"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::ObjBottom, "object.bottom", "Object Bottom"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::ObjSide,   "object.side",   "Object Side"),

    REGIST_ENUM_ITEM(ce::op::Comp::Selector::WorldSouth, "world.south", "World South"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::WorldNorth, "world.north", "World North"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::WorldWest,  "world.west",  "World West"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::WorldEast,  "world.east",  "World East"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::WorldUp,    "world.up",    "World Up"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::WorldDown,  "world.down",  "World Down"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::WorldSide,  "world.side",  "World Side"),

    REGIST_ENUM_ITEM(ce::op::Comp::Selector::Vertical,   "vertical",   "Vertical"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::Horizontal, "horizontal", "Horizontal"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::Aslant,     "aslant",     "Aslant"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::Nutant,     "nutant",     "Nutant"),

    REGIST_ENUM_ITEM(ce::op::Comp::Selector::Side, "side", "Side"),

    REGIST_ENUM_ITEM(ce::op::Comp::Selector::Border, "border", "Border"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::Inside, "inside", "Inside"),

    REGIST_ENUM_ITEM(ce::op::Comp::Selector::Eave,   "eave",   "Eave"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::Hip,    "hip",    "Hip"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::Valley, "valley", "Valley"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::Ridge,  "ridge",  "Ridge"),

    REGIST_ENUM_ITEM(ce::op::Comp::Selector::StreetFront, "street.front", "Street Front"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::StreetBack,  "street.back",  "Street Back"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::StreetLeft,  "street.left",  "Street Left"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::StreetRight, "street.right", "Street Right"),
    REGIST_ENUM_ITEM(ce::op::Comp::Selector::StreetSide,  "street.side",  "Street Side"),

    REGIST_ENUM_ITEM(ce::op::Comp::Selector::All, "all", "All"),

    REGIST_ENUM_ITEM(ce::op::Comp::Selector::Index, "index", "Index")
);
rttr::registration::class_<ce::op::Comp>("ce::comp")
.constructor<>()
#define PARM_FILEPATH "ce/op/Comp.parm.h"
#define PARM_NODE_CLASS ce::op::Comp
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::enumeration<ce::op::Offset::Selector>("cga_offset_selector")
(
    REGIST_ENUM_ITEM(ce::op::Offset::Selector::All,    "all",    "All"),
    REGIST_ENUM_ITEM(ce::op::Offset::Selector::Inside, "inside", "Inside"),
    REGIST_ENUM_ITEM(ce::op::Offset::Selector::Border, "border", "Border")
);
rttr::registration::class_<ce::op::Offset>("ce::offset")
.constructor<>()
#define PARM_FILEPATH "ce/op/Offset.parm.h"
#define PARM_NODE_CLASS ce::op::Offset
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::class_<ce::op::ShapeO>("ce::shapeo")
.constructor<>()
#define PARM_FILEPATH "ce/op/ShapeO.parm.h"
#define PARM_NODE_CLASS ce::op::ShapeO
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::enumeration<ce::op::Split::Axis>("cga_split_axis")
(
    REGIST_ENUM_ITEM(ce::op::Split::Axis::X, "x", "X"),
    REGIST_ENUM_ITEM(ce::op::Split::Axis::Y, "y", "Y"),
    REGIST_ENUM_ITEM(ce::op::Split::Axis::Z, "z", "Z")
);
rttr::registration::enumeration<ce::op::Split::SizeType>("cga_split_size")
(
    REGIST_ENUM_ITEM(ce::op::Split::SizeType::None,     "none",     "None"),
    REGIST_ENUM_ITEM(ce::op::Split::SizeType::Absolute, "absolute", "Absolute"),
    REGIST_ENUM_ITEM(ce::op::Split::SizeType::Relative, "relative", "Relative"),
    REGIST_ENUM_ITEM(ce::op::Split::SizeType::Floating, "floating", "Floating")
);
rttr::registration::class_<ce::op::Split::Part>("ce::split_part")
    .constructor()(rttr::policy::ctor::as_object)
    .property("type", &ce::op::Split::Part::size_type)
    .property("size",   &ce::op::Split::Part::size)
    .property("repeat", &ce::op::Split::Part::repeat)
;
rttr::registration::class_<ce::op::Split>("ce::split")
.constructor<>()
#define PARM_FILEPATH "ce/op/Split.parm.h"
#define PARM_NODE_CLASS ce::op::Split
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;

// transformations
rttr::registration::class_<ce::op::Center>("ce::center")
.constructor<>()
#define PARM_FILEPATH "ce/op/Center.parm.h"
#define PARM_NODE_CLASS ce::op::Center
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::class_<ce::op::Scale>("ce::s")
.constructor<>()
#define PARM_FILEPATH "ce/op/Scale.parm.h"
#define PARM_NODE_CLASS ce::op::Scale
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::class_<ce::op::TransScope>("ce::t")
.constructor<>()
#define PARM_FILEPATH "ce/op/TransScope.parm.h"
#define PARM_NODE_CLASS ce::op::TransScope
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;

// scope
rttr::registration::class_<ce::op::AlignScopeToGeo>("ce::alignScopeToGeometry")
.constructor<>()
#define PARM_FILEPATH "ce/op/AlignScopeToGeo.parm.h"
#define PARM_NODE_CLASS ce::op::AlignScopeToGeo
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;

// flow control
rttr::registration::class_<ce::op::NIL>("ce::NIL")
.constructor<>()
;
rttr::registration::class_<ce::op::Switch>("ce::switch")
.constructor<>()
#define PARM_FILEPATH "ce/op/Switch.parm.h"
#define PARM_NODE_CLASS ce::op::Switch
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;

// attributes
rttr::registration::class_<ce::op::Color>("ce::color")
.constructor<>()
#define PARM_FILEPATH "ce/op/Color.parm.h"
#define PARM_NODE_CLASS ce::op::Color
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::class_<ce::op::Set>("ce::set")
.constructor<>()
#define PARM_FILEPATH "ce/op/Set.parm.h"
#define PARM_NODE_CLASS ce::op::Set
#include <dag/rttr_prop_gen.h>
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;

}

namespace ce
{

void regist_rttr()
{
}

}