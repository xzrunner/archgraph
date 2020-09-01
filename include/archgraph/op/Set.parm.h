#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(Attribute, std::string, attribute, m_attr,  ())
PARAM_INFO(Value,     std::string,  value,    m_value, ())
