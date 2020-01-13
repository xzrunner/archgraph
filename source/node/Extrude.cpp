#include "cga/node/Extrude.h"
#include "cga/Geometry.h"
#include "cga/EvalExpr.h"
#include "cga/Variant.h"

#include <halfedge/Polyhedron.h>
#include <polymesh3/Polytope.h>

namespace cga
{
namespace node
{

void Extrude::Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
                      const EvalContext& ctx)
{
    if (in.empty() || !in[0]) {
        out.resize(1, nullptr);
        return;
    }

    assert(in.size() == 1);
    auto geo = std::make_shared<Geometry>(*in[0]);
    out.resize(1);
    out[0] = geo;

    auto poly = geo->GetPoly();
    if (!poly) {
        return;
    }
    auto topo_poly = poly->GetTopoPoly();
    if (!topo_poly) {
        return;
    }

    bool create_face[he::Polyhedron::ExtrudeMaxCount];
    create_face[he::Polyhedron::ExtrudeFront] = true;
    create_face[he::Polyhedron::ExtrudeBack]  = true;
    create_face[he::Polyhedron::ExtrudeSide]  = true;

    std::vector<he::TopoID> face_ids;
    auto& faces = topo_poly->GetLoops();
    face_ids.reserve(faces.Size());
    auto first_f = faces.Head();
    auto curr_f = first_f;
    do {
        face_ids.push_back(curr_f->ids);
        curr_f = curr_f->linked_next;
    } while (curr_f != first_f);

    if (!topo_poly->Extrude(m_distance, face_ids, create_face)) {
        return;
    }

    poly->BuildFromTopo();
}

void Extrude::Setup(const std::vector<cgac::ExprNodePtr>& parms,
                    const Rule::CompoundSel& selectors, const EvalContext& ctx)
{
    assert(parms.size() == 1 && selectors.sels.empty());
    auto var = EvalExpr::Eval(parms[0]);
    assert(var && var->Type() == VarType::Float);
    SetDistance(var->ToFloat());
}

}
}