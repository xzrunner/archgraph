#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(TransX, ce::RelativeFloat, tx, m_tx, ())
PARAM_INFO(TransY, ce::RelativeFloat, ty, m_ty, ())
PARAM_INFO(TransZ, ce::RelativeFloat, tz, m_tz, ())
