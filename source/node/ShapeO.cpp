#include "cga/node/ShapeO.h"
#include "cga/Geometry.h"
#include "cga/NodeHelper.h"
#include "cga/TopoPolyAdapter.h"

#include <halfedge/Polygon.h>
#include <polymesh3/Polytope.h>

namespace cga
{
namespace node
{

void ShapeO::Execute()
{
    if (m_front_width == 0) {
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

    std::vector<pm3::Polytope::PointPtr> border_pts, inside_pts;
    std::vector<pm3::Polytope::FacePtr>  border_faces, inside_faces;

    auto& src_points = prev_poly->Points();
    auto& src_faces = prev_poly->Faces();
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

    m_geo = std::make_shared<Geometry>(std::make_shared<pm3::Polytope>(border_pts, border_faces));
    m_rem_geo = std::make_shared<Geometry>(std::make_shared<pm3::Polytope>(inside_pts, inside_faces));
}

std::shared_ptr<Geometry> ShapeO::GetGeo(int idx) const
{
    if (idx < 0) {
        return m_geo;
    }

    switch (idx)
    {
    case OUT_SHAPE:
        return m_geo;
    case OUT_REMAINDER:
        return m_rem_geo;
    default:
        assert(0);
        return nullptr;
    }
}

}
}