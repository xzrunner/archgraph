#include "utility.h"

#include <SM_Cube.h>
#include <cga/CGA.h>
//#include <cga/Node.h>
#include <cga/Geometry.h>

#include <catch/catch.hpp>

namespace
{

void InitCGA()
{
    cga::CGA::Instance();
}

}

namespace test
{

void init()
{
    static bool inited = false;
    if (!inited)
    {
        InitCGA();

        inited = true;
    }
}

void check_aabb(const cga::Geometry& geo, const sm::vec3& min, const sm::vec3& max)
{
    auto poly = geo.GetPoly();
    REQUIRE(poly != nullptr);

    sm::cube aabb;
    for (auto& p : poly->Points()) {
        aabb.Combine(p->pos);
    }
    REQUIRE(aabb.Min()[0] == Approx(min.x));
    REQUIRE(aabb.Min()[1] == Approx(min.y));
    REQUIRE(aabb.Min()[2] == Approx(min.z));
    REQUIRE(aabb.Max()[0] == Approx(max.x));
    REQUIRE(aabb.Max()[1] == Approx(max.y));
    REQUIRE(aabb.Max()[2] == Approx(max.z));
}

void check_aabb_holes(const cga::Geometry& geo, const sm::vec3& min, const sm::vec3& max)
{
    auto poly = geo.GetPoly();
    REQUIRE(poly != nullptr);

    auto& pts = poly->Points();

    sm::cube aabb;
    for (auto& f : poly->Faces()) {
        for (auto& hole : f->holes) {
            for (auto& p : hole) {
                aabb.Combine(pts[p]->pos);
            }
        }
    }

    REQUIRE(aabb.Min()[0] == Approx(min.x));
    REQUIRE(aabb.Min()[1] == Approx(min.y));
    REQUIRE(aabb.Min()[2] == Approx(min.z));
    REQUIRE(aabb.Max()[0] == Approx(max.x));
    REQUIRE(aabb.Max()[1] == Approx(max.y));
    REQUIRE(aabb.Max()[2] == Approx(max.z));
}

void check_points_num(const cga::Geometry& geo, size_t num)
{
    auto poly = geo.GetPoly();
    REQUIRE(poly != nullptr);

    REQUIRE(poly->Points().size() == num);
}

void check_faces_num(const cga::Geometry& geo, size_t num)
{
    auto poly = geo.GetPoly();
    REQUIRE(poly != nullptr);

    REQUIRE(poly->Faces().size() == num);
}

}