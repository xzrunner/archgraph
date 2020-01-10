#include "cga/node/Offset.h"
#include "cga/Geometry.h"
#include "cga/TopoPolyAdapter.h"
#include "cga/EvalExpr.h"
#include "cga/Variant.h"

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

void Offset::Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
                     const EvalContext& ctx)
{
    assert(in.size() == 1);
    if (m_distance == 0) {
        out.resize(1);
        out[0] = in[0];
        return;
    }

    if (!in[0]) {
        out.resize(1, nullptr);
        return;
    }
    auto prev_poly = in[0]->GetPoly();
    if (!prev_poly) {
        out.resize(1);
        out[0] = in[0];
        return;
    }

    std::vector<pm3::Polytope::PointPtr> dst_pts;
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

        TopoPolyAdapter topo_poly(src_poly);

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
        topo_poly.GetPoly()->Offset(m_distance, keep);

        topo_poly.TransToPolymesh(dst_pts, dst_faces);
    }

    auto geo = std::make_shared<Geometry>(std::make_shared<pm3::Polytope>(dst_pts, dst_faces));
    out.resize(1);
    out[0] = geo;
}

void Offset::Setup(const std::vector<cgac::ExprNodePtr>& parms,
                   const Rule::CompoundSel& selectors, const EvalContext& ctx)
{
    assert(parms.size() == 1 && selectors.sels.empty());
    auto var = EvalExpr::Eval(parms[0]);
    assert(var && var->Type() == VarType::Float);
    SetDistance(var->ToFloat());
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
#endif // USE_CGAL

}
}