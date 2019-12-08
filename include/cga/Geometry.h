#pragma once

#include <SM_Vector.h>
#include <polymesh3/Polytope.h>

namespace cga
{

class Geometry
{
public:
    Geometry(const pm3::PolytopePtr& poly)
        : m_poly(poly)
    {
        m_color.MakeInvalid();
    }

    auto GetPoly() const { return m_poly; }

    auto& GetColor() const { return m_color; }
    void SetColor(const sm::vec3& color) { m_color = color; }

private:
    pm3::PolytopePtr m_poly = nullptr;

    sm::vec3 m_color;

}; // Geometry

}