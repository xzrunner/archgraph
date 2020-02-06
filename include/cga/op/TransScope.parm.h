#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(TransX, cga::RelativeFloat, tx, m_tx, ())
PARAM_INFO(TransY, cga::RelativeFloat, ty, m_ty, ())
PARAM_INFO(TransZ, cga::RelativeFloat, tz, m_tz, ())
