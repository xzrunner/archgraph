#include "cga/node/Extrude.h"
#include "cga/Geometry.h"
#include "cga/NodeHelper.h"

#include <halfedge/Polyhedron.h>
#include <polymesh3/Polytope.h>

namespace cga
{
namespace node
{

void Extrude::Execute()
{
    m_geo.reset();

    auto prev_geo = NodeHelper::GetInputGeo(*this, 0);
    if (!prev_geo) {
        return;
    }

    m_geo = std::make_shared<Geometry>(*prev_geo);

    auto poly = m_geo->GetPoly();
    if (!poly) {
        return;
    }
    auto he_poly = poly->GetHePoly();
    if (!he_poly) {
        return;
    }

    bool create_face[he::Polyhedron::ExtrudeMaxCount];
    create_face[he::Polyhedron::ExtrudeFront] = true;
    create_face[he::Polyhedron::ExtrudeBack]  = true;
    create_face[he::Polyhedron::ExtrudeSide]  = true;

    std::vector<he::TopoID> face_ids;
    auto& faces = he_poly->GetFaces();
    face_ids.reserve(faces.Size());
    auto first_f = faces.Head();
    auto curr_f = first_f;
    do {
        face_ids.push_back(curr_f->ids);
        curr_f = curr_f->linked_next;
    } while (curr_f != first_f);


    if (!he_poly->Extrude(m_distance, face_ids, create_face)) {
        return;
    }

    poly->BuildFromPoly();
}

}
}