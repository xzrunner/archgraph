#include "ce/op/PrimCube.h"
#include "ce/Geometry.h"
#include "ce/Variant.h"
#include "ce/EvalExpr.h"

#include <polymesh3/Polytope.h>

namespace ce
{
namespace op
{

void PrimCube::Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
                       const EvalContext& ctx)
{
    assert(in.size() == 0);

    std::vector<pm3::Polytope::FacePtr> faces;
    faces.reserve(6);

#ifdef BUILD_CENTER
    const auto h_sz = sm::vec3(m_width, m_height, m_depth) * 0.5f;
    const float xmin = -h_sz.x;
    const float xmax =  h_sz.x;
    const float ymin = -h_sz.y;
    const float ymax =  h_sz.y;
    const float zmin = -h_sz.z;
    const float zmax =  h_sz.z;
#else
    const float xmin = 0;
    const float xmax = m_width;
    const float ymin = 0;
    const float ymax = m_height;
    const float zmin = 0;
    const float zmax = m_depth;
#endif // BUILD_CENTER

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

    auto geo = std::make_shared<Geometry>(std::make_shared<pm3::Polytope>(faces));
    out.resize(1);
    out[0] = geo;
}

void PrimCube::Setup(const std::vector<cgac::ExprNodePtr>& parms,
                     const Rule::CompoundSel& selectors, const EvalContext& ctx)
{
    assert(parms.empty() || parms.size() == 3);
    if (parms.empty()) {
        return;
    }

    auto width  = EvalExpr::Eval(parms[0], ctx);
    auto height = EvalExpr::Eval(parms[1], ctx);
    auto depth  = EvalExpr::Eval(parms[2], ctx);
    SetWidth(check_float(width));
    SetHeight(check_float(height));
    SetDepth(check_float(depth));
}

}
}