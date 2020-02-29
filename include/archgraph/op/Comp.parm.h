#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(Type,     archgraph::op::Comp::Type,            type,      m_type,      (archgraph::op::Comp::Type::Faces))
PARAM_INFO(Selector, Array<archgraph::op::Comp::Selector>, selectors, m_selectors, ())
