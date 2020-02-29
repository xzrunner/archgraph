#include "utility.h"

#include <archgraph/op/Extrude.h>
#include <archgraph/op/Insert.h>
#include <archgraph/op/PrimCube.h>
#include <archgraph/op/PrimQuad.h>
#include <archgraph/op/PrimPoly.h>

#include <archgraph/EvalOp.h>
#include <archgraph/RuleLoader.h>
#include <archgraph/EvalRule.h>
#include <archgraph/Geometry.h>

#include <catch/catch.hpp>

TEST_CASE("extrude")
{
    test::init();

    archgraph::EvalOp eval;

    auto quad = std::make_shared<archgraph::op::PrimQuad>();
    quad->SetLength(1);
    quad->SetWidth(2);
    eval.AddOp(quad);

    auto extrude = std::make_shared<archgraph::op::Extrude>();
    extrude->SetDistance(3);
    eval.AddOp(extrude);

    eval.Connect({ quad, 0 }, { extrude, 0 });

    archgraph::EvalContext ctx;
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

    archgraph::EvalContext ctx;

    archgraph::RuleLoader loader;

    auto eval = std::make_shared<archgraph::EvalRule>();

    SECTION("quad")
    {
        loader.RunString(ctx, R"(
Lot-->
   extrude(10)
)", *eval/*, true*/);

        std::vector<archgraph::GeoPtr> _geos, geos;
        auto quad = std::make_shared<archgraph::op::PrimQuad>();
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

    archgraph::EvalOp eval;

    auto cube = std::make_shared<archgraph::op::PrimCube>();
    cube->SetWidth(1);
    cube->SetHeight(2);
    cube->SetDepth(3);
    eval.AddOp(cube);

    archgraph::EvalContext ctx;
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

    archgraph::EvalOp eval;

    auto quad = std::make_shared<archgraph::op::PrimQuad>();
    eval.AddOp(quad);

    auto insert = std::make_shared<archgraph::op::Insert>();
    insert->SetGeoPath("geo_path");
    eval.AddOp(insert);

    eval.Connect({ quad, 0 }, { insert, 0 });

    archgraph::EvalContext ctx;
    auto geos = eval.Eval(ctx);

    auto geo = test::query_geo(geos, insert);
    REQUIRE(geo->GetFilepath() == "geo_path");
}

TEST_CASE("quad")
{
    test::init();

    archgraph::EvalOp eval;

    auto quad = std::make_shared<archgraph::op::PrimQuad>();
    quad->SetWidth(1);
    quad->SetLength(2);
    eval.AddOp(quad);

    archgraph::EvalContext ctx;
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

    archgraph::EvalOp eval;

    auto poly = std::make_shared<archgraph::op::PrimPoly>();
    poly->SetVertices({
        {0, 0},
        {3, 0},
        {2, 1},
        {1, 1},
    });
    eval.AddOp(poly);

    archgraph::EvalContext ctx;
    auto geos = eval.Eval(ctx);

    auto geo = test::query_geo(geos, poly);
    test::check_aabb(*geo, { 0, 0, 0 }, { 3, 0, 1 });
}