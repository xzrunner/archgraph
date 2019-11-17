#pragma once

#include <polymesh3/typedef.h>

namespace cga
{

class Geometry
{
public:

    void SetPoly(const pm3::PolytopePtr& poly) { m_poly = poly; }
    auto GetPoly() const { return m_poly; }

private:
    pm3::PolytopePtr m_poly = nullptr;

}; // Geometry

}