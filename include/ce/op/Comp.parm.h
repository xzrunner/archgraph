#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(Type,     ce::op::Comp::Type,            type,      m_type,      (ce::op::Comp::Type::Faces))
PARAM_INFO(Selector, Array<ce::op::Comp::Selector>, selectors, m_selectors, ())
