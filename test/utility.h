#pragma once

#include <SM_Vector.h>
#include <cga/typedef.h>

#include <map>
#include <vector>

namespace test
{

void init();

void check_aabb(const cga::Geometry& geo, const sm::vec3& min, const sm::vec3& max);
void check_aabb_holes(const cga::Geometry& geo, const sm::vec3& min, const sm::vec3& max);

void check_points_num(const cga::Geometry& geo, size_t num);
void check_faces_num(const cga::Geometry& geo, size_t num);

void check_single_face_norm(const cga::Geometry& geo, const sm::vec3& norm);
void check_single_face_area(const cga::Geometry& geo, float area);

cga::GeoPtr query_geo(const std::map<cga::NodePtr, std::vector<cga::GeoPtr>>& geos, 
    const cga::NodePtr& node, size_t out_id = 0);

}