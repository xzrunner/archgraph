#include "cga/node/Offset.h"
#include "cga/Geometry.h"
#include "cga/NodeHelper.h"

#include <halfedge/Polyhedron.h>
#include <polymesh3/Polytope.h>

namespace cga
{
namespace node
{

void Offset::Execute()
{
    m_geo.reset();

    auto prev_geo = NodeHelper::GetInputGeo(*this, 0);
    if (!prev_geo) {
        return;
    }

    m_geo = std::make_shared<Geometry>(*prev_geo);

    auto poly = m_geo->GetPoly();
    assert(poly);
    auto he_poly = poly->GetHePoly();
    assert(he_poly);
    auto& faces = he_poly->GetFaces();
    auto first_f = faces.Head();
    auto curr_f = first_f;
    do {
        std::vector<sm::vec3> verts;

        auto first_e = curr_f->edge;
        auto curr_e = first_e;
        do {
            verts.push_back(curr_e->vert->position);
            curr_e = curr_e->next;
        } while (curr_e != first_e);

        auto norm = CalcFaceNormal(verts);

        int ptr = 0;
        first_e = curr_f->edge;
        curr_e = first_e;
        do {
            auto& p0 = curr_e->vert->position;
            auto& p1 = curr_e->next->vert->position;
            auto offset = (p1 - p0).Cross(norm).Normalized() * m_distance;
            verts[ptr] += offset;
            verts[(ptr + 1) % verts.size()] += offset;

            curr_e = curr_e->next;
            ++ptr;
        } while (curr_e != first_e);

        ptr = 0;
        first_e = curr_f->edge;
        curr_e = first_e;
        do {
            curr_e->vert->position = verts[ptr++];
            curr_e = curr_e->next;
        } while (curr_e != first_e);

        curr_f = curr_f->linked_next;
    } while (curr_f != first_f);

    poly->BuildFromPoly();

    //auto& faces = poly->Faces();
    //auto& pts = poly->Points();
    //for (auto& face : faces)
    //{
    //    std::vector<sm::vec3> verts;
    //    verts.reserve(face->points.size());
    //    for (auto& p : face->points) {
    //        verts.push_back(pts[p]->pos);
    //    }

    //    auto& norm = face->plane.normal;
    //    for (size_t i = 0, n = face->points.size(); i < n - 1; ++i)
    //    {
    //        auto& p0 = pts[face->points[i]];
    //        auto& p1 = pts[face->points[i + 1]];
    //        auto offset = (p1->pos - p0->pos).Cross(norm).Normalized() * m_distance;
    //        verts[i] += offset;
    //        verts[i + 1] += offset;
    //    }

    //    for (size_t i = 0, n = verts.size(); i < n; ++i) {
    //        pts[face->points[i]]->pos = verts[i];
    //    }
    //}

    //poly->BuildFromFaces();
}

sm::vec3 Offset::CalcFaceNormal(const std::vector<sm::vec3>& verts)
{
    assert(verts.size() > 2);
    auto& p0 = verts[0];
    auto& p1 = verts[1];
    auto& p2 = verts[2];
    return (p1 - p0).Cross(p2 - p0).Normalized();
}

}
}