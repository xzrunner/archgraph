#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(Type,     cga::op::Comp::Type,            type,      m_type,      (cga::op::Comp::Type::Faces))
PARAM_INFO(Selector, Array<cga::op::Comp::Selector>, selectors, m_selectors, ())
