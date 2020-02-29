#define EXE_FILEPATH "archgraph/op_include_gen.h"
#include "archgraph/op_regist_cfg.h"
#undef EXE_FILEPATH

#include <rttr/registration>

#define REGIST_NODE_TYPE(type, name)                           \
	rttr::registration::class_<archgraph::op::type>("archgraph::"#name)  \
		.constructor<>()                                       \
	;

#define REGIST_ENUM_ITEM(type, name, label) \
    rttr::value(name, type),                \
    rttr::metadata(type, label)             \

RTTR_REGISTRATION
{

// base

rttr::registration::class_<dag::Node<archgraph::OpVarType>::Port>("archgraph::Operation::Port")
	.property("var", &dag::Node<archgraph::OpVarType>::Port::var)
;

rttr::registration::class_<archgraph::Operation>("archgraph::Operation")
	.method("GetImports", &archgraph::Operation::GetImports)
	.method("GetExports", &archgraph::Operation::GetExports)
;

rttr::registration::class_<archgraph::RelativeFloat>("archgraph::rfloat")
    .property("value",    &archgraph::RelativeFloat::value)
    .property("relative", &archgraph::RelativeFloat::relative)
;

#define EXE_FILEPATH "archgraph/op_rttr_gen.h"
#include "archgraph/op_regist_cfg.h"
#undef EXE_FILEPATH

// creation

rttr::registration::enumeration<archgraph::op::Extrude::ExtrusionType>("cga_extrusion_type")
(
    REGIST_ENUM_ITEM(archgraph::op::Extrude::ExtrusionType::WorldUp,        "world.up",         "World Up"),
    REGIST_ENUM_ITEM(archgraph::op::Extrude::ExtrusionType::WorldUpFlatTop, "world.up.flatTop", "World Up Flat Top"),
    REGIST_ENUM_ITEM(archgraph::op::Extrude::ExtrusionType::FaceNormal,     "face.normal",      "Face Normal"),
    REGIST_ENUM_ITEM(archgraph::op::Extrude::ExtrusionType::VertexNormal,   "vertex.normal",    "Vertex Normal")
);

rttr::registration::enumeration<archgraph::op::Insert::UpAxisOfGeo>("cga_insert_up_axis")
(
    REGIST_ENUM_ITEM(archgraph::op::Insert::UpAxisOfGeo::UpY, "y_up", "Y Up"),
    REGIST_ENUM_ITEM(archgraph::op::Insert::UpAxisOfGeo::UpZ, "z_up", "Z Up")
);
rttr::registration::enumeration<archgraph::op::Insert::InsertMode>("cga_insert_mode")
(
    REGIST_ENUM_ITEM(archgraph::op::Insert::InsertMode::AlignSizeAndPosition,  "align_size_and_position",  "Align Size And Position"),
    REGIST_ENUM_ITEM(archgraph::op::Insert::InsertMode::KeepSizeAndPosition,   "keep_size_and_position",   "Keep Size And Position"),
    REGIST_ENUM_ITEM(archgraph::op::Insert::InsertMode::KeepSizeAlignPosition, "keep_size_align_position", "Keep Size Align Position")
);

// subdivision
rttr::registration::enumeration<archgraph::op::Comp::Type>("cga_comp_type")
(
    REGIST_ENUM_ITEM(archgraph::op::Comp::Type::Faces,     "f",  "Faces"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Type::Edges,     "e",  "Edges"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Type::FaceEdges, "fe", "Face Edges"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Type::Vertices,  "v",  "Vertices"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Type::Groups,    "g",  "Groups"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Type::Materials, "m",  "Materials"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Type::Holes,     "h",  "Holes")
);
rttr::registration::enumeration<archgraph::op::Comp::Selector>("cga_comp_selector")
(
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::Front,  "front",  "Front"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::Back,   "back",   "Back"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::Left,   "left",   "Left"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::Right,  "right",  "Right"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::Top,    "top",    "Top"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::Bottom, "bottom", "Bottom"),

    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::ObjFront,  "object.front",  "Object Front"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::ObjBack,   "object.back",   "Object Back"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::ObjLeft,   "object.left",   "Object Left"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::ObjRight,  "object.right",  "Object Right"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::ObjTop,    "object.top",    "Object Top"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::ObjBottom, "object.bottom", "Object Bottom"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::ObjSide,   "object.side",   "Object Side"),

    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::WorldSouth, "world.south", "World South"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::WorldNorth, "world.north", "World North"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::WorldWest,  "world.west",  "World West"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::WorldEast,  "world.east",  "World East"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::WorldUp,    "world.up",    "World Up"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::WorldDown,  "world.down",  "World Down"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::WorldSide,  "world.side",  "World Side"),

    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::Vertical,   "vertical",   "Vertical"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::Horizontal, "horizontal", "Horizontal"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::Aslant,     "aslant",     "Aslant"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::Nutant,     "nutant",     "Nutant"),

    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::Side, "side", "Side"),

    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::Border, "border", "Border"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::Inside, "inside", "Inside"),

    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::Eave,   "eave",   "Eave"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::Hip,    "hip",    "Hip"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::Valley, "valley", "Valley"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::Ridge,  "ridge",  "Ridge"),

    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::StreetFront, "street.front", "Street Front"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::StreetBack,  "street.back",  "Street Back"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::StreetLeft,  "street.left",  "Street Left"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::StreetRight, "street.right", "Street Right"),
    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::StreetSide,  "street.side",  "Street Side"),

    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::All, "all", "All"),

    REGIST_ENUM_ITEM(archgraph::op::Comp::Selector::Index, "index", "Index")
);

rttr::registration::enumeration<archgraph::op::Offset::Selector>("cga_offset_selector")
(
    REGIST_ENUM_ITEM(archgraph::op::Offset::Selector::All,    "all",    "All"),
    REGIST_ENUM_ITEM(archgraph::op::Offset::Selector::Inside, "inside", "Inside"),
    REGIST_ENUM_ITEM(archgraph::op::Offset::Selector::Border, "border", "Border")
);
rttr::registration::enumeration<archgraph::op::Split::Axis>("cga_split_axis")
(
    REGIST_ENUM_ITEM(archgraph::op::Split::Axis::X, "x", "X"),
    REGIST_ENUM_ITEM(archgraph::op::Split::Axis::Y, "y", "Y"),
    REGIST_ENUM_ITEM(archgraph::op::Split::Axis::Z, "z", "Z")
);
rttr::registration::enumeration<archgraph::op::Split::SizeType>("cga_split_size")
(
    REGIST_ENUM_ITEM(archgraph::op::Split::SizeType::None,     "none",     "None"),
    REGIST_ENUM_ITEM(archgraph::op::Split::SizeType::Absolute, "absolute", "Absolute"),
    REGIST_ENUM_ITEM(archgraph::op::Split::SizeType::Relative, "relative", "Relative"),
    REGIST_ENUM_ITEM(archgraph::op::Split::SizeType::Floating, "floating", "Floating")
);
rttr::registration::class_<archgraph::op::Split::Part>("archgraph::split_part")
    .constructor()(rttr::policy::ctor::as_object)
    .property("type", &archgraph::op::Split::Part::size_type)
    .property("size",   &archgraph::op::Split::Part::size)
    .property("repeat", &archgraph::op::Split::Part::repeat)
;

}

namespace archgraph
{

void regist_rttr()
{
}

}