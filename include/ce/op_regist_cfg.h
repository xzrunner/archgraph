#ifndef EXE_FILEPATH
#error "You must define EXE_FILEPATH macro before include this file"
#endif

// creation

#define PARM_OP_CLASS Extrude
#define PARM_OP_NAME extrude
#include EXE_FILEPATH

#define PARM_OP_CLASS Insert
#define PARM_OP_NAME i
#include EXE_FILEPATH

#define PARM_OP_CLASS PrimCube
#define PARM_OP_NAME prim_cube
#include EXE_FILEPATH

#define PARM_OP_CLASS PrimQuad
#define PARM_OP_NAME prim_quad
#include EXE_FILEPATH

#define PARM_OP_CLASS PrimPoly
#define PARM_OP_NAME prim_poly
#include EXE_FILEPATH

// subdivision

#define PARM_OP_CLASS Comp
#define PARM_OP_NAME comp
#include EXE_FILEPATH

#define PARM_OP_CLASS Offset
#define PARM_OP_NAME offset
#include EXE_FILEPATH

#define PARM_OP_CLASS ShapeO
#define PARM_OP_NAME shapeo
#include EXE_FILEPATH

#define PARM_OP_CLASS Split
#define PARM_OP_NAME split
#include EXE_FILEPATH

// transformations

#define PARM_OP_CLASS Center
#define PARM_OP_NAME center
#include EXE_FILEPATH

#define PARM_OP_CLASS Scale
#define PARM_OP_NAME s
#include EXE_FILEPATH

#define PARM_OP_CLASS TransScope
#define PARM_OP_NAME t
#include EXE_FILEPATH

// scope

#define PARM_OP_CLASS AlignScopeToGeo
#define PARM_OP_NAME alignScopeToGeometry
#include EXE_FILEPATH

// flow control

#define PARM_OP_CLASS NIL
#define PARM_OP_NAME NIL
#include EXE_FILEPATH

#define PARM_OP_CLASS Switch
#define PARM_OP_NAME switch
#include EXE_FILEPATH

// attributes

#define PARM_OP_CLASS Color
#define PARM_OP_NAME color
#include EXE_FILEPATH

#define PARM_OP_CLASS Set
#define PARM_OP_NAME set
#include EXE_FILEPATH
