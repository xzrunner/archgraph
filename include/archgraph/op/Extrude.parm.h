#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(Type,     archgraph::op::Extrude::ExtrusionType, type,     m_type,     (archgraph::op::Extrude::ExtrusionType::FaceNormal))
PARAM_INFO(Distance, Float,                           distance, m_distance, (0.0f))
