#include "cga/node/ShapeO.h"
#include "cga/Geometry.h"
#include "cga/TopoPolyAdapter.h"

#include <halfedge/Polygon.h>
#include <polymesh3/Polytope.h>

namespace cga
{
namespace node
{

void ShapeO::Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
                     const EvalContext& ctx)
{
    if (in.empty() || !in[0]) {
        out.resize(2, nullptr);
        return;
    }

    assert(in.size() == 1);
    auto prev_poly = in[0]->GetPoly();
    if (m_front_width == 0 || !prev_poly)
    {
        out.resize(2);
        out[0] = in[0];
        out[1] = in[0];
        return;
    }

    std::vector<pm3::Polytope::PointPtr> border_pts, inside_pts;
    std::vector<pm3::Polytope::FacePtr>  border_faces, inside_faces;

    auto& src_points = prev_poly->Points();
    auto& src_faces  = prev_poly->Faces();
    for (auto& src_f : src_faces)
    {
        std::vector<sm::vec3> src_poly;
        src_poly.reserve(src_f->border.size());
        for (auto& p : src_f->border) {
            src_poly.push_back(src_points[p]->pos);
        }

        TopoPolyAdapter border(src_poly);
        border.GetPoly()->Offset(-m_front_width, he::Polygon::KeepType::KeepBorder);
        border.TransToPolymesh(border_pts, border_faces);

        TopoPolyAdapter inside(src_poly);
        inside.GetPoly()->Offset(-m_front_width, he::Polygon::KeepType::KeepInside);
        inside.TransToPolymesh(inside_pts, inside_faces);
    }

    auto shape = std::make_shared<Geometry>(std::make_shared<pm3::Polytope>(border_pts, border_faces));
    auto remainder = std::make_shared<Geometry>(std::make_shared<pm3::Polytope>(inside_pts, inside_faces));
    out.resize(2);
    out[OUT_SHAPE]     = shape;
    out[OUT_REMAINDER] = remainder;
}

}
}