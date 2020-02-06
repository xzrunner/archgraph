#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(Axis,   cga::op::Split::Axis,        axis,   m_axis,   (cga::op::Split::Axis::X))
PARAM_INFO(Parts,  Array<cga::op::Split::Part>, parts,  m_parts,  ())
PARAM_INFO(Repeat, Bool,                        repeat, m_repeat, (false))
