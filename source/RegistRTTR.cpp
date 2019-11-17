// creation
#include "cga/node/PrimCube.h"

#define REGIST_NODE_TYPE(type, name)                           \
	rttr::registration::class_<cga::node::type>("cga::"#name)  \
		.constructor<>()                                       \
	;

RTTR_REGISTRATION
{

// creation
REGIST_NODE_TYPE(PrimCube, prim_cube)

}

namespace cga
{

void regist_rttr()
{
}

}