#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(Type,     Type,            type,      m_type,      (Type::Faces))
PARAM_INFO(Selector, Array<Selector>, selectors, m_selectors, ())
