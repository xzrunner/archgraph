#include "utility.h"

#include <SM_Cube.h>
#include <SM_Calc.h>
#include <cga/CGA.h>
#include <cga/Geometry.h>

#include <catch/catch.hpp>

namespace
{

void InitCGA()
{
    cga::CGA::Instance();
}

void check_equal(const sm::vec3& v0, const sm::vec3& v1)
{
    REQUIRE(v0.x == Approx(v1.x));
    REQUIRE(v0.y == Approx(v1.y));
    REQUIRE(v0.z == Approx(v1.z));
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

void check_single_face_norm(const cga::Geometry& geo, const sm::vec3& norm)
{
    auto& faces = geo.GetPoly()->Faces();
    REQUIRE(faces.size() == 1);
    check_equal(faces[0]->plane.normal, norm);
}

void check_single_face_area(const cga::Geometry& geo, float area)
{
    auto& faces = geo.GetPoly()->Faces();
    REQUIRE(faces.size() == 1);

    auto calc_loop_area = [](const std::vector<size_t>& loop, const std::vector<pm3::Polytope::PointPtr>& points) -> float
    {
        std::vector<sm::vec3> verts;
        verts.reserve(points.size());
        for (auto& i : loop) {
            verts.push_back(points[i]->pos);
        }
        return sm::get_polygon_area(verts);
    };

    auto& pts = geo.GetPoly()->Points();
    float a = calc_loop_area(faces[0]->border, pts);
    for (auto& hole : faces[0]->holes) {
        a -= calc_loop_area(hole, pts);
    }
}

cga::GeoPtr query_geo(const std::map<cga::OpPtr, std::vector<cga::GeoPtr>>& geos,
                      const cga::OpPtr& node, size_t out_id)
{
    auto itr = geos.find(node);
    if (itr == geos.end()) {
        return nullptr;
    } else {
        assert(out_id < itr->second.size());
        return itr->second[out_id];
    }
}

void check_color(const cga::Geometry& geo, const sm::vec3& col)
{
    auto& c = geo.GetColor();
    REQUIRE(c.x == col.x);
    REQUIRE(c.y == col.y);
    REQUIRE(c.z == col.z);
}

}