#include "utility.h"

#include <cga/EvalNode.h>
#include <cga/RuleLoader.h>
#include <cga/EvalRule.h>
#include <cga/Geometry.h>

#include <cga/node/NIL.h>
#include <cga/node/PrimCube.h>

#include <catch/catch.hpp>

TEST_CASE("NIL")
{
    test::init();

    cga::EvalContext ctx;

    cga::RuleLoader loader;

    auto eval = std::make_shared<cga::EvalRule>();

    std::vector<cga::GeoPtr> _geos, geos;
    auto cube = std::make_shared<cga::node::PrimCube>();
    cube->SetWidth(10);
    cube->SetHeight(10);
    cube->SetDepth(10);
    cube->Execute(_geos, geos, ctx);
    assert(geos.size() == 1);

    SECTION("Using NIL to create holes")
    {
        loader.RunString(R"(
Lot-->
   split(x){ { ~1 : X
             | ~1 : NIL }*
           | ~1 : X }

X -->
    color("#ffffff")

)", *eval/*, true*/);

        geos = eval->Eval(geos);
        REQUIRE(geos.size() == 9);
        test::check_color(*geos[6], sm::vec3(1, 1, 1));
        test::check_aabb(*geos[0], sm::vec3(0, 0, 0), sm::vec3(1.125f, 10, 10));
        REQUIRE(geos[1] == nullptr);
        test::check_aabb(*geos[2], sm::vec3(2.25f, 0, 0), sm::vec3(3.375f, 10, 10));
        REQUIRE(geos[3] == nullptr);
        test::check_aabb(*geos[4], sm::vec3(4.5f, 0, 0), sm::vec3(5.625f, 10, 10));
        REQUIRE(geos[5] == nullptr);
        test::check_aabb(*geos[6], sm::vec3(6.75f, 0, 0), sm::vec3(7.875f, 10, 10));
        REQUIRE(geos[7] == nullptr);
        test::check_aabb(*geos[8], sm::vec3(9, 0, 0), sm::vec3(10, 10, 10));
    }
}