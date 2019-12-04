// creation
#include "cga/node/PrimCube.h"

#define REGIST_NODE_TYPE(type, name)                           \
	rttr::registration::class_<cga::node::type>("cga::"#name)  \
		.constructor<>()                                       \
	;

RTTR_REGISTRATION
{

// base

rttr::registration::class_<hdiop::Node<cga::NodeVarType>::Port>("cga::Node::Port")
	.property("var", &hdiop::Node<cga::NodeVarType>::Port::var)
;

rttr::registration::class_<cga::Node>("cga::Node")
	.method("GetImports", &cga::Node::GetImports)
	.method("GetExports", &cga::Node::GetExports)
;

// creation
REGIST_NODE_TYPE(PrimCube, prim_cube)

}

namespace cga
{

void regist_rttr()
{
}

}