#pragma once

#include <SM_Vector.h>
#include <archgraph/typedef.h>

#include <map>
#include <vector>

namespace test
{

void init();

void check_aabb(const archgraph::Geometry& geo, const sm::vec3& min, const sm::vec3& max);
void check_aabb_holes(const archgraph::Geometry& geo, const sm::vec3& min, const sm::vec3& max);

void check_points_num(const archgraph::Geometry& geo, size_t num);
void check_faces_num(const archgraph::Geometry& geo, size_t num);

void check_single_face_norm(const archgraph::Geometry& geo, const sm::vec3& norm);
void check_single_face_area(const archgraph::Geometry& geo, float area);

archgraph::GeoPtr query_geo(const std::map<archgraph::OpPtr, std::vector<archgraph::GeoPtr>>& geos,
    const archgraph::OpPtr& node, size_t out_id = 0);

void check_color(const archgraph::Geometry& geo, const sm::vec3& col);

}