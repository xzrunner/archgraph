#include "cga/node/PrimCube.h"
#include "cga/Geometry.h"

#include <polymesh3/Polytope.h>

namespace cga
{
namespace node
{

void PrimCube::Execute()
{
    BuildModel();
}

void PrimCube::BuildModel()
{
    std::vector<pm3::Polytope::FacePtr> faces;
    faces.reserve(6);

    const auto h_sz = sm::vec3(m_width, m_height, m_depth) * 0.5f;
    const float xmin = -h_sz.x;
    const float xmax =  h_sz.x;
    const float ymin = -h_sz.y;
    const float ymax =  h_sz.y;
    const float zmin = -h_sz.z;
    const float zmax =  h_sz.z;

    const sm::vec3 top_left_front (xmin, ymax, zmin);
    const sm::vec3 top_left_back  (xmin, ymax, zmax);
    const sm::vec3 top_right_front(xmax, ymax, zmin);
    const sm::vec3 top_right_back (xmax, ymax, zmax);
    const sm::vec3 btm_left_front (xmin, ymin, zmin);
    const sm::vec3 btm_left_back  (xmin, ymin, zmax);
    const sm::vec3 btm_right_front(xmax, ymin, zmin);
    const sm::vec3 btm_right_back (xmax, ymin, zmax);

    auto top = std::make_shared<pm3::Polytope::Face>();
    top->plane = sm::Plane(top_left_front, top_left_back, top_right_back);
    faces.push_back(top);

    auto bottom = std::make_shared<pm3::Polytope::Face>();
    bottom->plane = sm::Plane(btm_left_front, btm_right_front, btm_right_back);
    faces.push_back(bottom);

    auto left = std::make_shared<pm3::Polytope::Face>();
    left->plane = sm::Plane(top_left_back, top_left_front, btm_left_front);
    faces.push_back(left);

    auto right = std::make_shared<pm3::Polytope::Face>();
    right->plane = sm::Plane(top_right_front, top_right_back, btm_right_back);
    faces.push_back(right);

    auto front = std::make_shared<pm3::Polytope::Face>();
    front->plane = sm::Plane(top_left_front, top_right_front, btm_right_front);
    faces.push_back(front);

    auto back = std::make_shared<pm3::Polytope::Face>();
    back->plane = sm::Plane(top_right_back, top_left_back, btm_left_back);
    faces.push_back(back);

    m_geo = std::make_shared<Geometry>(std::make_shared<pm3::Polytope>(faces));
}

}
}