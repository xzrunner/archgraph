// creation
#include "cga/node/PrimCube.h"
#include "cga/node/PrimQuad.h"

#include <rttr/registration>

#define REGIST_NODE_TYPE(type, name)                           \
	rttr::registration::class_<cga::node::type>("cga::"#name)  \
		.constructor<>()                                       \
	;

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

}

namespace cga
{

void regist_rttr()
{
}

}