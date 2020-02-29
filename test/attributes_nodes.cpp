#include "utility.h"

#include <archgraph/op/Color.h>

#include <archgraph/op/PrimQuad.h>

#include <archgraph/EvalOp.h>
#include <archgraph/RuleLoader.h>
#include <archgraph/EvalRule.h>
#include <archgraph/Geometry.h>

#include <catch/catch.hpp>

TEST_CASE("color")
{
    test::init();

    archgraph::EvalOp eval;

    auto quad = std::make_shared<archgraph::op::PrimQuad>();
    eval.AddOp(quad);

    auto color = std::make_shared<archgraph::op::Color>();
    color->SetColor({ 1, 0.5f, 0 });
    eval.AddOp(color);

    eval.Connect({ quad, 0 }, { color, 0 });

    archgraph::EvalContext ctx;
    auto geos = eval.Eval(ctx);

    auto geo = test::query_geo(geos, color);
    test::check_color(*geo, { 1, 0.5f, 0 });
}

TEST_CASE("Set")
{
    test::init();

    archgraph::EvalContext ctx;

    archgraph::RuleLoader loader;

    auto eval = std::make_shared<archgraph::EvalRule>();

    std::vector<archgraph::GeoPtr> _geos, geos;
    auto quad = std::make_shared<archgraph::op::PrimQuad>();
    quad->Execute(_geos, geos, ctx);
    assert(geos.size() == 1);

    loader.RunString(ctx, R"(
attr floorNumber = -1

const floorHeight = 4

Init-->
   extrude(30)
   comp(f) {side : Side }

Side-->
   split(y) { ~floorHeight: Floor }*

Floor-->
   set(floorNumber, split.index)
   split(x) { ~1   : Wall
            | {~3  : Tile }*
            | ~1   : Wall }
Tile-->
   split(x) { 0.5  : Wall
            | ~1   : MidTile
            | 0.5  : Wall }
MidTile-->
   split(y) { 0.5  : Wall
            | ~1   : Window
            | 0.5  : Wall }

Window-->
   case (floorNumber % 3 == 0):
        color(1.0, 0.0, 1.0)
        X
   else:
        X

Wall --> print("wall")
X    --> print("x")

)", *eval/*, true*/);

    geos = eval->Eval(geos, ctx);

    int zz = 0;
    //REQUIRE(geos.size() == 9);
    //test::check_color(*geos[6], sm::vec3(1, 1, 1));
    //test::check_aabb(*geos[0], sm::vec3(0, 0, 0), sm::vec3(1.125f, 10, 10));
    //REQUIRE(geos[1] == nullptr);
    //test::check_aabb(*geos[2], sm::vec3(2.25f, 0, 0), sm::vec3(3.375f, 10, 10));
    //REQUIRE(geos[3] == nullptr);
    //test::check_aabb(*geos[4], sm::vec3(4.5f, 0, 0), sm::vec3(5.625f, 10, 10));
    //REQUIRE(geos[5] == nullptr);
    //test::check_aabb(*geos[6], sm::vec3(6.75f, 0, 0), sm::vec3(7.875f, 10, 10));
    //REQUIRE(geos[7] == nullptr);
    //test::check_aabb(*geos[8], sm::vec3(9, 0, 0), sm::vec3(10, 10, 10));
}

TEST_CASE("Set debug")
{
    test::init();

    archgraph::EvalContext ctx;

    archgraph::RuleLoader loader;

    auto eval = std::make_shared<archgraph::EvalRule>();
    loader.RunString(ctx, R"(
attr floorNumber = -1

const floorHeight = 4

Init-->
   prim_quad(16, 16)
   extrude(30)
   comp(f) {side : Side }

Side-->
   split(y) { ~floorHeight: Floor }*

Floor-->
   set(floorNumber, split.index)
   split(x) { ~1   : Wall
            | {~3  : Tile }*
            | ~1   : Wall }
Tile-->
   split(x) { 0.5  : Wall
            | ~1   : MidTile
            | 0.5  : Wall }
MidTile-->
   split(y) { 0.5  : Wall
            | ~1   : Window
            | 0.5  : Wall }

Window-->
   case (floorNumber % 3 == 0):
        color(1.0, 0.0, 1.0)
        X
   else:
        X

Wall --> print("wall")
X    --> print("x")

)", *eval/*, true*/);

    auto geos = eval->Eval({}, ctx);

    int zz = 0;
    //REQUIRE(geos.size() == 9);
    //test::check_color(*geos[6], sm::vec3(1, 1, 1));
    //test::check_aabb(*geos[0], sm::vec3(0, 0, 0), sm::vec3(1.125f, 10, 10));
    //REQUIRE(geos[1] == nullptr);
    //test::check_aabb(*geos[2], sm::vec3(2.25f, 0, 0), sm::vec3(3.375f, 10, 10));
    //REQUIRE(geos[3] == nullptr);
    //test::check_aabb(*geos[4], sm::vec3(4.5f, 0, 0), sm::vec3(5.625f, 10, 10));
    //REQUIRE(geos[5] == nullptr);
    //test::check_aabb(*geos[6], sm::vec3(6.75f, 0, 0), sm::vec3(7.875f, 10, 10));
    //REQUIRE(geos[7] == nullptr);
    //test::check_aabb(*geos[8], sm::vec3(9, 0, 0), sm::vec3(10, 10, 10));
}