#include "cga/node/PrimQuad.h"
#include "cga/Geometry.h"

#include <polymesh3/Polytope.h>

namespace cga
{
namespace node
{

void PrimQuad::Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
                       const EvalContext& ctx)
{
    assert(in.size() == 0);

    const auto hw = m_width * 0.5f;
    const auto hh = m_length * 0.5f;

    std::vector<pm3::Polytope::PointPtr> points;
    points.reserve(4);
    points.push_back(std::make_shared<pm3::Polytope::Point>(sm::vec3(-hw, 0, -hh)));
    points.push_back(std::make_shared<pm3::Polytope::Point>(sm::vec3(-hw, 0,  hh)));
    points.push_back(std::make_shared<pm3::Polytope::Point>(sm::vec3( hw, 0,  hh)));
    points.push_back(std::make_shared<pm3::Polytope::Point>(sm::vec3( hw, 0, -hh)));

    auto face = std::make_shared<pm3::Polytope::Face>();
    face->border = { 0, 1, 2, 3 };
    std::vector<pm3::Polytope::FacePtr> faces;
    faces.push_back(face);

    auto geo = std::make_shared<Geometry>(std::make_shared<pm3::Polytope>(points, faces));
    out.resize(1);
    out[0] = geo;
}

}
}