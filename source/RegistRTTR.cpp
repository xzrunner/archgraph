// creation
#include "cga/node/Extrude.h"
#include "cga/node/PrimCube.h"
#include "cga/node/PrimQuad.h"
// subdivision
#include "cga/node/Offset.h"
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