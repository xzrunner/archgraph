#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(Axis,   archgraph::op::Split::Axis,             axis,   m_axis,   (archgraph::op::Split::Axis::X))
PARAM_INFO(Parts, std::vector<archgraph::op::Split::Part>, parts,  m_parts,  ())
PARAM_INFO(Repeat, bool,                                   repeat, m_repeat, (false))
