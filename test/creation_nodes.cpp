#include "utility.h"

#include <ce/op/Extrude.h>
#include <ce/op/Insert.h>
#include <ce/op/PrimCube.h>
#include <ce/op/PrimQuad.h>
#include <ce/op/PrimPoly.h>

#include <ce/EvalOp.h>
#include <ce/RuleLoader.h>
#include <ce/EvalRule.h>
#include <ce/Geometry.h>

#include <catch/catch.hpp>

TEST_CASE("extrude")
{
    test::init();

    ce::EvalOp eval;

    auto quad = std::make_shared<ce::op::PrimQuad>();
    quad->SetLength(1);
    quad->SetWidth(2);
    eval.AddOp(quad);

    auto extrude = std::make_shared<ce::op::Extrude>();
    extrude->SetDistance(3);
    eval.AddOp(extrude);

    eval.Connect({ quad, 0 }, { extrude, 0 });

    ce::EvalContext ctx;
    auto geos = eval.Eval(ctx);

    auto geo = test::query_geo(geos, extrude);
    test::check_points_num(*geo, 8);
    test::check_faces_num(*geo, 6);
#ifdef BUILD_CENTER
    test::check_aabb(*geo, { -1, 0, -0.5f }, { 1, 3, 0.5f });
#else
    test::check_aabb(*geo, { 0, 0, 0 }, { 2, 3, 1 });
#endif // BUILD_CENTER
}

TEST_CASE("extrude rule")
{
    test::init();

    ce::EvalContext ctx;

    ce::RuleLoader loader;

    auto eval = std::make_shared<ce::EvalRule>();

    SECTION("quad")
    {
        loader.RunString(ctx, R"(
Lot-->
   extrude(10)
)", *eval/*, true*/);

        std::vector<ce::GeoPtr> _geos, geos;
        auto quad = std::make_shared<ce::op::PrimQuad>();
        quad->SetWidth(2);
        quad->SetLength(3);
        quad->Execute(_geos, geos, ctx);
        assert(geos.size() == 1);

        geos = eval->Eval(geos, ctx);

        REQUIRE(geos.size() == 1);
        test::check_aabb(*geos[0], { 0, 0, 0 }, { 2, 10, 3 });
    }
}

TEST_CASE("cube")
{
    test::init();

    ce::EvalOp eval;

    auto cube = std::make_shared<ce::op::PrimCube>();
    cube->SetWidth(1);
    cube->SetHeight(2);
    cube->SetDepth(3);
    eval.AddOp(cube);

    ce::EvalContext ctx;
    auto geos = eval.Eval(ctx);

    auto geo = test::query_geo(geos, cube);
#ifdef BUILD_CENTER
    test::check_aabb(*geo, { -0.5f, -1, -1.5f }, { 0.5f, 1, 1.5f });
#else
    test::check_aabb(*geo, { 0, 0, 0 }, { 1, 2, 3 });
#endif // BUILD_CENTER
}

TEST_CASE("insert")
{
    test::init();

    ce::EvalOp eval;

    auto quad = std::make_shared<ce::op::PrimQuad>();
    eval.AddOp(quad);

    auto insert = std::make_shared<ce::op::Insert>();
    insert->SetGeoPath("geo_path");
    eval.AddOp(insert);

    eval.Connect({ quad, 0 }, { insert, 0 });

    ce::EvalContext ctx;
    auto geos = eval.Eval(ctx);

    auto geo = test::query_geo(geos, insert);
    REQUIRE(geo->GetFilepath() == "geo_path");
}

TEST_CASE("quad")
{
    test::init();

    ce::EvalOp eval;

    auto quad = std::make_shared<ce::op::PrimQuad>();
    quad->SetWidth(1);
    quad->SetLength(2);
    eval.AddOp(quad);

    ce::EvalContext ctx;
    auto geos = eval.Eval(ctx);

    auto geo = test::query_geo(geos, quad);
#ifdef BUILD_CENTER
    test::check_aabb(*geo, { -0.5f, 0, -1 }, { 0.5f, 0, 1 });
#else
    test::check_aabb(*geo, { 0, 0, 0 }, { 1, 0, 2 });
#endif // BUILD_CENTER
}

TEST_CASE("poly")
{
    test::init();

    ce::EvalOp eval;

    auto poly = std::make_shared<ce::op::PrimPoly>();
    poly->SetVertices({
        {0, 0},
        {3, 0},
        {2, 1},
        {1, 1},
    });
    eval.AddOp(poly);

    ce::EvalContext ctx;
    auto geos = eval.Eval(ctx);

    auto geo = test::query_geo(geos, poly);
    test::check_aabb(*geo, { 0, 0, 0 }, { 3, 0, 1 });
}