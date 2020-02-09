#pragma once

#include <SM_Vector.h>
#include <ce/typedef.h>

#include <map>
#include <vector>

namespace test
{

void init();

void check_aabb(const ce::Geometry& geo, const sm::vec3& min, const sm::vec3& max);
void check_aabb_holes(const ce::Geometry& geo, const sm::vec3& min, const sm::vec3& max);

void check_points_num(const ce::Geometry& geo, size_t num);
void check_faces_num(const ce::Geometry& geo, size_t num);

void check_single_face_norm(const ce::Geometry& geo, const sm::vec3& norm);
void check_single_face_area(const ce::Geometry& geo, float area);

ce::GeoPtr query_geo(const std::map<ce::OpPtr, std::vector<ce::GeoPtr>>& geos,
    const ce::OpPtr& node, size_t out_id = 0);

void check_color(const ce::Geometry& geo, const sm::vec3& col);

}