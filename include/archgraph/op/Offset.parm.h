#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(Selector, archgraph::op::Offset::Selector, selector, m_selector, (archgraph::op::Offset::Selector::All))
PARAM_INFO(Distance, float,                           distance, m_distance, (0.0f))
