#define XSTR(s) STR(s)
#define STR(s) #s

#ifndef PARM_OP_CLASS
#error "You must define PARM_OP_CLASS macro before include this file"
#endif

#ifndef PARM_OP_NAME
#error "You must define PARM_OP_NAME macro before include this file"
#endif

#ifndef PARM_FILEPATH_PARM
#define PARM_FILEPATH_PARM archgraph/op/##PARM_OP_CLASS##.parm.h
#endif

#define RTTR_NAME archgraph::##PARM_OP_NAME

rttr::registration::class_<archgraph::op::PARM_OP_CLASS>(XSTR(RTTR_NAME))
.constructor<>()
#define PARM_FILEPATH XSTR(PARM_FILEPATH_PARM)
#define PARM_NODE_CLASS archgraph::op::##PARM_OP_CLASS
#include <dag/rttr_prop_gen.h>
#undef PARM_FILEPATH
#undef PARM_NODE_CLASS
;

#undef PARM_OP_NAME
#undef PARM_OP_CLASS
#undef PARM_FILEPATH_PARM
