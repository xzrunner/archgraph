#define XSTR(s) STR(s)
#define STR(s) #s

#ifndef PARM_OP_CLASS
#error "You must define PARM_OP_CLASS macro before include this file"
#endif

#ifndef PARM_FILEPATH_H
#define PARM_FILEPATH_H ce/op/##PARM_OP_CLASS##.h
#endif

#include XSTR(PARM_FILEPATH_H)

#undef PARM_OP_NAME
#undef PARM_OP_CLASS
#undef PARM_FILEPATH_H
