#include "archgraph/op/PrimPoly.h"
#include "archgraph/Geometry.h"

#include <polymesh3/Polytope.h>

namespace archgraph
{
namespace op
{

void PrimPoly::Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
                       const EvalContext& ctx)
{
    assert(in.size() == 0);

    std::vector<pm3::Polytope::PointPtr> points;
    points.resize(m_vertices.size());

    auto face = std::make_shared<pm3::Polytope::Face>();
    face->border.resize(m_vertices.size());
    for (size_t i = 0, n = m_vertices.size(); i < n; ++i) 
    {
        sm::vec3 pos;
        pos.x = m_vertices[i].x;
        pos.y = 0;
        pos.z = m_vertices[i].y;
        points[i] = std::make_shared<pm3::Polytope::Point>(pos);

        face->border[i] = i;
    }
    std::vector<pm3::Polytope::FacePtr> faces;
    faces.push_back(face);

    auto geo = std::make_shared<Geometry>(std::make_shared<pm3::Polytope>(points, faces));
    out.resize(1);
    out[0] = geo;
}

}
}