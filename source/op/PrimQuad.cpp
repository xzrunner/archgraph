#include "cga/op/PrimQuad.h"
#include "cga/Geometry.h"
#include "cga/Variant.h"
#include "cga/EvalExpr.h"

#include <polymesh3/Polytope.h>

namespace cga
{
namespace op
{

void PrimQuad::Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
                       const EvalContext& ctx)
{
    assert(in.size() == 0);

    std::vector<pm3::Polytope::PointPtr> points;
    points.reserve(4);
#ifdef BUILD_CENTER
    const auto hw = m_width * 0.5f;
    const auto hh = m_length * 0.5f;

    points.push_back(std::make_shared<pm3::Polytope::Point>(sm::vec3(-hw, 0, -hh)));
    points.push_back(std::make_shared<pm3::Polytope::Point>(sm::vec3(-hw, 0,  hh)));
    points.push_back(std::make_shared<pm3::Polytope::Point>(sm::vec3( hw, 0,  hh)));
    points.push_back(std::make_shared<pm3::Polytope::Point>(sm::vec3( hw, 0, -hh)));
#else
    points.push_back(std::make_shared<pm3::Polytope::Point>(sm::vec3(0,       0, 0)));
    points.push_back(std::make_shared<pm3::Polytope::Point>(sm::vec3(0,       0, m_length)));
    points.push_back(std::make_shared<pm3::Polytope::Point>(sm::vec3(m_width, 0, m_length)));
    points.push_back(std::make_shared<pm3::Polytope::Point>(sm::vec3(m_width, 0, 0)));
#endif // BUILD_CENTER

    auto face = std::make_shared<pm3::Polytope::Face>();
    face->border = { 0, 1, 2, 3 };
    std::vector<pm3::Polytope::FacePtr> faces;
    faces.push_back(face);

    auto geo = std::make_shared<Geometry>(std::make_shared<pm3::Polytope>(points, faces));
    out.resize(1);
    out[0] = geo;
}

void PrimQuad::Setup(const std::vector<cgac::ExprNodePtr>& parms,
                     const Rule::CompoundSel& selectors, const EvalContext& ctx)
{
    assert(parms.empty() || parms.size() == 2);
    if (parms.empty()) {
        return;
    }

    auto width  = EvalExpr::Eval(parms[0], ctx);
    auto length = EvalExpr::Eval(parms[1], ctx);
    SetWidth(check_float(width));
    SetLength(check_float(length));
}

}
}