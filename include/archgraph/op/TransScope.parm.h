#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(TransX, archgraph::RelativeFloat, tx, m_tx, ())
PARAM_INFO(TransY, archgraph::RelativeFloat, ty, m_ty, ())
PARAM_INFO(TransZ, archgraph::RelativeFloat, tz, m_tz, ())
