#include "ce/Geometry.h"

namespace ce
{

void Geometry::AddAttr(const std::string& name, const std::shared_ptr<Variant>& value)
{
    m_attrs.insert({ name, value });
}

bool Geometry::SetAttr(const std::string& name, const std::shared_ptr<Variant>& value)
{
    auto itr = m_attrs.find(name);
    if (itr == m_attrs.end()) {
        return false;
    } else {
        itr->second = value;
        return true;
    }
}

std::shared_ptr<Variant>
Geometry::QueryAttr(const std::string& name) const
{
    auto itr = m_attrs.find(name);
    return itr == m_attrs.end() ? nullptr : itr->second;
}

}