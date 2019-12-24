#include "cga/node/Split.h"
#include "cga/Geometry.h"

#include <halfedge/Polyhedron.h>
#include <halfedge/Utility.h>

namespace cga
{
namespace node
{

void Split::Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out)
{
    assert(in.size() == 1);
    if (!in[0]) {
        return;
    }

    auto knife_pos = CalcKnifePos(in[0]);
    assert(knife_pos.size() + 1 == m_parts.size());

    sm::vec3 normal;
    switch (m_axis)
    {
    case Axis::X:
        normal.Set(1, 0, 0);
        break;
    case Axis::Y:
        normal.Set(0, 1, 0);
        break;
    case Axis::Z:
        normal.Set(0, 0, 1);
        break;
    default:
        assert(0);
    }

    auto prev_geo = in[0]->GetPoly()->GetTopoPoly();
    for (auto& pos : knife_pos)
    {
        sm::Plane plane(normal, pos);
        prev_geo->Clip(plane, he::Polyhedron::KeepType::KeepAll, true);

        std::vector<he::Polyhedron::Face> up, down;
        for (auto& face : prev_geo->GetFaces())
        {
            auto st = he::Utility::CalcFacePlaneStatus(face, plane);
            switch (st)
            {
            case he::Utility::FaceStatus::Above:
                up.push_back(face);
                break;
            case he::Utility::FaceStatus::Below:
                down.push_back(face);
                break;
            case he::Utility::FaceStatus::Inside:
                up.push_back(face);
                down.push_back(face);
                break;
            default:
                assert(0);
            }
        }

        auto part = std::make_shared<he::Polyhedron>(up);
        out.push_back(std::make_shared<Geometry>(std::make_shared<pm3::Polytope>(part)));

        prev_geo = std::make_shared<he::Polyhedron>(down);
    }

    out.push_back(std::make_shared<Geometry>(std::make_shared<pm3::Polytope>(prev_geo)));
}

std::vector<float> Split::CalcKnifePos(const GeoPtr& geo) const
{
    std::vector<float> sizes(m_parts.size(), 0);

    float tot_len, start_pos;
    auto& aabb = geo->GetPoly()->GetTopoPoly()->GetAABB();
    switch (m_axis)
    {
    case Axis::X:
        tot_len = aabb.xmax - aabb.xmin;
        start_pos = aabb.xmin;
        break;
    case Axis::Y:
        tot_len = aabb.ymax - aabb.ymin;
        start_pos = aabb.ymin;
        break;
    case Axis::Z:
        tot_len = aabb.zmax - aabb.zmin;
        start_pos = aabb.zmin;
        break;
    }

    float left = tot_len;
    for (size_t i = 0, n = m_parts.size(); i < n; ++i)
    {
        float abs_sz = 0;

        auto& p = m_parts[i];
        switch (p.type)
        {
        case SizeType::Absolute:
            abs_sz = p.size;
            break;
        case SizeType::Relative:
            abs_sz = tot_len * p.size;
            break;
        }

        if (abs_sz <= left) {
            left -= abs_sz;
        } else {
            abs_sz = left;
            left = 0;
        }

        sizes[i] = abs_sz;

        if (left == 0) {
            break;
        }
    }

    std::vector<float> pos;
    float curr_pos = start_pos;
    for (size_t i = 0, n = sizes.size(); i < n - 1; ++i)
    {
        curr_pos += sizes[i];
        pos.push_back(curr_pos);
    }
    return pos;
}

}
}