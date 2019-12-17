#pragma once

#include <SM_Vector.h>

namespace cga { class Geometry; }

namespace test
{

void init();

void check_aabb(const cga::Geometry& geo, const sm::vec3& min, const sm::vec3& max);
void check_aabb_holes(const cga::Geometry& geo, const sm::vec3& min, const sm::vec3& max);

void check_points_num(const cga::Geometry& geo, size_t num);
void check_faces_num(const cga::Geometry& geo, size_t num);

}