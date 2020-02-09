#pragma once

#include <SM_Vector.h>
#include <polymesh3/Polytope.h>

namespace ce
{

class Variant;

class Geometry
{
public:
    Geometry(const pm3::PolytopePtr& poly)
        : m_poly(poly)
    {
        m_color.MakeInvalid();
    }

    Geometry(const std::vector<std::shared_ptr<Geometry>>& children)
        : m_children(children)
    {
    }

    auto GetPoly() const { return m_poly; }

    auto& GetColor() const { return m_color; }
    void SetColor(const sm::vec3& color) { m_color = color; }

    auto& GetFilepath() const { return m_filepath; }
    void SetFilepath(const std::string& filepath) { m_filepath = filepath; }

    auto& GetChildren() const { return m_children; }

    void AddAttr(const std::string& name, const std::shared_ptr<Variant>& value);
    bool SetAttr(const std::string& name, const std::shared_ptr<Variant>& value);
    std::shared_ptr<Variant> QueryAttr(const std::string& name) const;

private:
    pm3::PolytopePtr m_poly = nullptr;

    sm::vec3 m_color;

    std::string m_filepath;

    std::vector<std::shared_ptr<Geometry>> m_children;

    std::map<std::string, std::shared_ptr<Variant>> m_attrs;

}; // Geometry

}