#include "cga/node/Offset.h"
#include "cga/Geometry.h"
#include "cga/NodeHelper.h"

#include <halfedge/Polyhedron.h>
#include <halfedge/Polygon.h>
#include <polymesh3/Polytope.h>
#include <SM_Calc.h>

#ifdef USE_CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/create_straight_skeleton_2.h>
#include <CGAL/create_offset_polygons_2.h>
#endif // USE_CGAL

namespace cga
{
namespace node
{

void Offset::Execute()
{
    if (m_distance == 0) {
        return;
    }

    m_geo.reset();

    auto prev_geo = NodeHelper::GetInputGeo(*this, 0);
    if (!prev_geo) {
        return;
    }
    auto prev_poly = prev_geo->GetPoly();
    if (!prev_poly) {
        return;
    }

    std::vector<pm3::Polytope::PointPtr> dst_points;
    std::vector<pm3::Polytope::FacePtr>  dst_faces;

    auto& src_points = prev_poly->Points();
    auto& src_faces = prev_poly->Faces();
    for (auto& src_f : src_faces)
    {
        std::vector<sm::vec3> src_poly;
        src_poly.reserve(src_f->border.size());
        for (auto& p : src_f->border) {
            src_poly.push_back(src_points[p]->pos);
        }
        auto src_norm = sm::calc_face_normal(src_poly);

        std::vector<std::vector<sm::vec3>> borders;
        std::vector<std::vector<sm::vec3>> holes;
        OffsetPolygon(src_poly, borders, holes);

        auto face = std::make_shared<pm3::Polytope::Face>();
        face->border.reserve(borders.size());
        assert(borders.size() == 1);
        for (auto& p : borders[0])
        {
            face->border.push_back(dst_points.size());
            dst_points.push_back(std::make_shared<pm3::Polytope::Point>(p));
        }
        for (auto& hole : holes)
        {
            std::vector<size_t> dst_hole;
            dst_hole.reserve(hole.size());
            for (auto& p : hole)
            {
                dst_hole.push_back(dst_points.size());
                dst_points.push_back(std::make_shared<pm3::Polytope::Point>(p));
            }
            face->holes.push_back(dst_hole);
        }
        dst_faces.push_back(face);
    }

    m_geo = std::make_shared<Geometry>(std::make_shared<pm3::Polytope>(dst_points, dst_faces));
}

#ifdef USE_CGAL

std::vector<std::vector<sm::vec3>>
Offset::OffsetPolygon(const std::vector<sm::vec3>& src_poly, float offset)
{
    typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
    typedef K::Point_2                   Point;
    typedef CGAL::Polygon_2<K>           Polygon_2;
    typedef CGAL::Straight_skeleton_2<K> Ss;
    typedef boost::shared_ptr<Polygon_2> PolygonPtr;
    typedef boost::shared_ptr<Ss> SsPtr;
    typedef std::vector<PolygonPtr> PolygonPtrVector;

    auto norm = sm::calc_face_normal(src_poly);
    auto rot = sm::mat4(sm::Quaternion::CreateFromVectors(norm, sm::vec3(0, 1, 0)));
    auto inv_rot = rot.Inverted();

    Polygon_2 poly;
    for (auto itr = src_poly.rbegin(); itr != src_poly.rend(); ++itr) {
        auto p = rot * *itr;
        poly.push_back(Point(p.x, p.z));
    }

    auto ret_polys = [&inv_rot](const PolygonPtrVector& polys) -> std::vector<std::vector<sm::vec3>>
    {
        std::vector<std::vector<sm::vec3>> dst_polys;
        dst_polys.reserve(polys.size());
        for (auto& polygon : polys)
        {
            std::vector<sm::vec3> dst_poly;
            dst_poly.reserve(polygon->size());
            for (auto itr = polygon->vertices_begin(); itr != polygon->vertices_end(); ++itr) {
                sm::vec3 p(static_cast<float>(itr->x()), 0.0f, static_cast<float>(itr->y()));
                dst_poly.push_back(inv_rot * p);
            }
            std::reverse(dst_poly.begin(), dst_poly.end());
            dst_polys.push_back(dst_poly);
        }
        return dst_polys;
    };

    if (offset < 0)
    {
        double d_offset = -offset;

        SsPtr ss = CGAL::create_interior_straight_skeleton_2(poly);

        PolygonPtrVector offset_polygons = CGAL::create_offset_polygons_2<Polygon_2>(d_offset, *ss);

        return ret_polys(offset_polygons);
    }
    else
    {
        double d_offset = offset;

        SsPtr ss = CGAL::create_exterior_straight_skeleton_2(d_offset * 2, poly);

        PolygonPtrVector offset_polygons = CGAL::create_offset_polygons_2<Polygon_2>(d_offset, *ss);

        return ret_polys(offset_polygons);
    }
}

#else

void Offset::OffsetPolygon(const std::vector<sm::vec3>& src_poly,
                           std::vector<std::vector<sm::vec3>>& dst_borders,
                           std::vector<std::vector<sm::vec3>>& dst_holes) const
{
    auto norm = sm::calc_face_normal(src_poly);
    auto rot = sm::mat4(sm::Quaternion::CreateFromVectors(norm, sm::vec3(0, 1, 0)));
    auto inv_rot = rot.Inverted();

    std::vector<std::pair<he::TopoID, sm::vec2>> vertices;
    std::vector<std::pair<he::TopoID, std::vector<size_t>>> borders, holes;

    std::vector<size_t> loop;
    size_t idx = 0;
    loop.reserve(src_poly.size());
    for (auto itr = src_poly.rbegin(); itr != src_poly.rend(); ++itr)
    {
        auto p = rot * *itr;
        vertices.push_back({ he::TopoID(), sm::vec2(p.x, p.z) });
        loop.push_back(idx++);
    }
    borders.push_back({ he::TopoID(), loop });
    he::Polygon poly(vertices, borders, holes);

    he::Polygon::KeepType keep;
    switch (m_selector)
    {
    case Selector::All:
        keep = he::Polygon::KeepType::KeepAll;
        break;
    case Selector::Inside:
        keep = he::Polygon::KeepType::KeepInside;
        break;
    case Selector::Border:
        keep = he::Polygon::KeepType::KeepBorder;
        break;
    default:
        assert(0);
    }
    poly.Offset(m_distance, keep);

    auto dump_loop = [](const he::face2& loop, const sm::mat4& mt) -> std::vector<sm::vec3>
    {
        std::vector<sm::vec3> dst_poly;

        auto first_e = loop.edge;
        auto curr_e = first_e;
        do {
            auto& p = curr_e->vert->position;
            dst_poly.push_back(mt * sm::vec3(p.x, 0, p.y));

            curr_e = curr_e->next;
        } while (curr_e != first_e);

        std::reverse(dst_poly.begin(), dst_poly.end());

        return dst_poly;
    };

    auto first_f = poly.GetBorders().Head();
    auto curr_f = first_f;
    do {
        dst_borders.push_back(dump_loop(*curr_f, inv_rot));
        curr_f = curr_f->linked_next;
    } while (curr_f != first_f);

    first_f = poly.GetHoles().Head();
    if (first_f)
    {
        curr_f = first_f;
        do {
            dst_holes.push_back(dump_loop(*curr_f, inv_rot));
            curr_f = curr_f->linked_next;
        } while (curr_f != first_f);
    }
}

#endif // USE_CGAL

}
}