#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(Axis,  Axis,        axis,  m_axis,  (Axis::X))
PARAM_INFO(Parts, Array<Part>, parts, m_parts, ())
