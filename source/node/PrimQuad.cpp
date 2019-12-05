#include "cga/node/PrimQuad.h"
#include "cga/Geometry.h"

#include <polymesh3/Geometry.h>

namespace cga
{
namespace node
{

void PrimQuad::Execute()
{
    BuildModel();
}

void PrimQuad::BuildModel()
{
    const auto hw = m_width * 0.5f;
    const auto hh = m_length * 0.5f;

    std::vector<pm3::PointPtr> points;
    points.reserve(4);
    points.push_back(std::make_shared<pm3::Point>(sm::vec3(-hw, 0, -hh)));
    points.push_back(std::make_shared<pm3::Point>(sm::vec3( hw, 0, -hh)));
    points.push_back(std::make_shared<pm3::Point>(sm::vec3( hw, 0,  hh)));
    points.push_back(std::make_shared<pm3::Point>(sm::vec3(-hw, 0,  hh)));

    auto face = std::make_shared<pm3::Face>();
    face->points = { 0, 1, 2, 3 };
    std::vector<pm3::FacePtr> faces;
    faces.push_back(face);

    m_geo = std::make_shared<Geometry>(std::make_shared<pm3::Polytope>(points, faces));
}

}
}