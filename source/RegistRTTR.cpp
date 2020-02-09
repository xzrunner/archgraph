#define EXE_FILEPATH "ce/op_include_gen.h"
#include "ce/op_regist_cfg.h"
#undef EXE_FILEPATH

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

#define EXE_FILEPATH "ce/op_rttr_gen.h"
#include "ce/op_regist_cfg.h"
#undef EXE_FILEPATH

// creation

rttr::registration::enumeration<ce::op::Extrude::ExtrusionType>("cga_extrusion_type")
(
    REGIST_ENUM_ITEM(ce::op::Extrude::ExtrusionType::WorldUp,        "world.up",         "World Up"),
    REGIST_ENUM_ITEM(ce::op::Extrude::ExtrusionType::WorldUpFlatTop, "world.up.flatTop", "World Up Flat Top"),
    REGIST_ENUM_ITEM(ce::op::Extrude::ExtrusionType::FaceNormal,     "face.normal",      "Face Normal"),
    REGIST_ENUM_ITEM(ce::op::Extrude::ExtrusionType::VertexNormal,   "vertex.normal",    "Vertex Normal")
);

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

rttr::registration::enumeration<ce::op::Offset::Selector>("cga_offset_selector")
(
    REGIST_ENUM_ITEM(ce::op::Offset::Selector::All,    "all",    "All"),
    REGIST_ENUM_ITEM(ce::op::Offset::Selector::Inside, "inside", "Inside"),
    REGIST_ENUM_ITEM(ce::op::Offset::Selector::Border, "border", "Border")
);
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

}

namespace ce
{

void regist_rttr()
{
}

}