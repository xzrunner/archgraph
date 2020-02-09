#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(GeoPath,     String,                       geo_path,    m_geo_path,    ())
PARAM_INFO(UpAxisOfGeo, ce::op::Insert::UpAxisOfGeo, up_axis,     m_up_axis,     (ce::op::Insert::UpAxisOfGeo::UpY))
PARAM_INFO(InsertMode,  ce::op::Insert::InsertMode,  insert_mode, m_insert_mode, (ce::op::Insert::InsertMode::AlignSizeAndPosition))
