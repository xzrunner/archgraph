#include "cga/Geometry.h"

namespace cga
{

void Geometry::AddAttr(const std::string& name, const std::shared_ptr<Variant>& value)
{
    m_attrs.insert({ name, value });
}

std::shared_ptr<Variant>
Geometry::QueryAttr(const std::string& name) const
{
    auto itr = m_attrs.find(name);
    return itr == m_attrs.end() ? nullptr : itr->second;
}

}