#include "utility.h"

#include <cga/node/Extrude.h>
#include <cga/node/PrimCube.h>
#include <cga/node/PrimQuad.h>
#include <cga/node/PrimPoly.h>

#include <cga/EvalNode.h>
#include <cga/RuleLoader.h>
#include <cga/EvalRule.h>
#include <cga/Geometry.h>

#include <catch/catch.hpp>

TEST_CASE("extrude")
{
    test::init();

    cga::EvalNode eval;

    auto quad = std::make_shared<cga::node::PrimQuad>();
    quad->SetLength(1);
    quad->SetWidth(2);
    eval.AddNode(quad);

    auto extrude = std::make_shared<cga::node::Extrude>();
    extrude->SetDistance(3);
    eval.AddNode(extrude);

    eval.Connect({ quad, 0 }, { extrude, 0 });

    auto geos = eval.Eval();

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

    cga::EvalContext ctx;

    cga::RuleLoader loader;
    auto eval = std::make_shared<cga::EvalRule>();

    SECTION("quad")
    {
        loader.RunString(R"(
Lot-->
   extrude(10)
)", *eval/*, true*/);

        std::vector<cga::GeoPtr> _geos, geos;
        auto quad = std::make_shared<cga::node::PrimQuad>();
        quad->SetWidth(2);
        quad->SetLength(3);
        quad->Execute(_geos, geos, ctx);
        assert(geos.size() == 1);

        geos = eval->Eval(geos);

        REQUIRE(geos.size() == 1);
        test::check_aabb(*geos[0], { 0, 0, 0 }, { 2, 10, 3 });
    }
}

TEST_CASE("cube")
{
    test::init();

    cga::EvalNode eval;

    auto cube = std::make_shared<cga::node::PrimCube>();
    cube->SetWidth(1);
    cube->SetHeight(2);
    cube->SetDepth(3);
    eval.AddNode(cube);

    auto geos = eval.Eval();

    auto geo = test::query_geo(geos, cube);
#ifdef BUILD_CENTER
    test::check_aabb(*geo, { -0.5f, -1, -1.5f }, { 0.5f, 1, 1.5f });
#else
    test::check_aabb(*geo, { 0, 0, 0 }, { 1, 2, 3 });
#endif // BUILD_CENTER
}

TEST_CASE("quad")
{
    test::init();

    cga::EvalNode eval;

    auto quad = std::make_shared<cga::node::PrimQuad>();
    quad->SetWidth(1);
    quad->SetLength(2);
    eval.AddNode(quad);

    auto geos = eval.Eval();

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

    cga::EvalNode eval;

    auto poly = std::make_shared<cga::node::PrimPoly>();
    poly->SetVertices({
        {0, 0},
        {3, 0},
        {2, 1},
        {1, 1},
    });
    eval.AddNode(poly);

    auto geos = eval.Eval();

    auto geo = test::query_geo(geos, poly);
    test::check_aabb(*geo, { 0, 0, 0 }, { 3, 0, 1 });
}