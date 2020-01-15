#include "utility.h"

#include <cga/op/Scale.h>
#include <cga/op/TransScope.h>
#include <cga/op/PrimCube.h>
#include <cga/op/PrimQuad.h>

#include <cga/EvalOp.h>
#include <cga/RuleLoader.h>
#include <cga/EvalRule.h>
#include <cga/Geometry.h>

#include <catch/catch.hpp>

TEST_CASE("scale")
{
    test::init();

    cga::EvalOp eval;

    auto cube = std::make_shared<cga::op::PrimCube>();
    cube->SetWidth(1);
    cube->SetHeight(2);
    cube->SetDepth(3);
    eval.AddOp(cube);

    SECTION("absolute")
    {
        auto s = std::make_shared<cga::op::Scale>();
        s->SetScaleX({ 3, false });
        s->SetScaleY({ 2, false });
        s->SetScaleZ({ 1, false });
        eval.AddOp(s);

        eval.Connect({ cube, 0 }, { s, 0 });

        auto geos = eval.Eval();

        auto geo = test::query_geo(geos, s);
        test::check_points_num(*geo, 8);
        test::check_faces_num(*geo, 6);
        test::check_aabb(*geo, { 0, 0, 0 }, { 3, 2, 1 });
    }

    SECTION("relative")
    {
        auto s = std::make_shared<cga::op::Scale>();
        s->SetScaleX(3);
        s->SetScaleY(2);
        s->SetScaleZ(1);
        eval.AddOp(s);

        eval.Connect({ cube, 0 }, { s, 0 });

        auto geos = eval.Eval();

        auto geo = test::query_geo(geos, s);
        test::check_points_num(*geo, 8);
        test::check_faces_num(*geo, 6);
        test::check_aabb(*geo, { 0, 0, 0 }, { 3, 4, 3 });
    }
}

TEST_CASE("scale rule")
{
    test::init();

    cga::EvalContext ctx;

    cga::RuleLoader loader;

    auto eval = std::make_shared<cga::EvalRule>();

    std::vector<cga::GeoPtr> _geos, geos;
    auto quad = std::make_shared<cga::op::PrimQuad>();
    quad->SetWidth(2);
    quad->SetLength(3);
    quad->Execute(_geos, geos, ctx);
    assert(geos.size() == 1);

    SECTION("absolute")
    {
        loader.RunString(R"(
Lot-->
   extrude(10)
   s(5,5,5)
)", *eval/*, true*/);

        geos = eval->Eval(geos, ctx);

        REQUIRE(geos.size() == 1);
        test::check_aabb(*geos[0], { 0, 0, 0 }, { 5, 5, 5 });
    }

    SECTION("relative")
    {
        loader.RunString(R"(
Lot-->
   extrude(10)
   s('0.5,'1,'1.5)
)", *eval/*, true*/);

        geos = eval->Eval(geos, ctx);

        REQUIRE(geos.size() == 1);
        test::check_aabb(*geos[0], { 0, 0, 0 }, { 1, 10, 4.5f });
    }
}

TEST_CASE("trans_scope")
{
    test::init();

    cga::EvalOp eval;

    auto cube = std::make_shared<cga::op::PrimCube>();
    cube->SetWidth(1);
    cube->SetHeight(2);
    cube->SetDepth(3);
    eval.AddOp(cube);

    SECTION("absolute")
    {
        auto t = std::make_shared<cga::op::TransScope>();
        t->SetTransX({ 3, false });
        t->SetTransY({ 2, false });
        t->SetTransZ({ 1, false });
        eval.AddOp(t);

        eval.Connect({ cube, 0 }, { t, 0 });

        auto geos = eval.Eval();

        auto geo = test::query_geo(geos, t);
        test::check_points_num(*geo, 8);
        test::check_faces_num(*geo, 6);
        test::check_aabb(*geo, { 3, 2, 1 }, { 4, 4, 4 });
    }

    SECTION("relative")
    {
        auto t = std::make_shared<cga::op::TransScope>();
        t->SetTransX(3);
        t->SetTransY(2);
        t->SetTransZ(1);
        eval.AddOp(t);

        eval.Connect({ cube, 0 }, { t, 0 });

        auto geos = eval.Eval();

        auto geo = test::query_geo(geos, t);
        test::check_points_num(*geo, 8);
        test::check_faces_num(*geo, 6);
        test::check_aabb(*geo, { 3, 4, 3 }, { 4, 6, 6 });
    }
}

TEST_CASE("trans_scope rule")
{
    test::init();

    cga::EvalContext ctx;

    cga::RuleLoader loader;

    auto eval = std::make_shared<cga::EvalRule>();

    std::vector<cga::GeoPtr> _geos, geos;
    auto quad = std::make_shared<cga::op::PrimQuad>();
    quad->SetWidth(2);
    quad->SetLength(3);
    quad->Execute(_geos, geos, ctx);
    assert(geos.size() == 1);

    SECTION("absolute")
    {
        loader.RunString(R"(
Lot-->
   extrude(10)
   t(1,3,5)
)", *eval/*, true*/);

        geos = eval->Eval(geos, ctx);

        REQUIRE(geos.size() == 1);
        test::check_aabb(*geos[0], { 1, 3, 5 }, { 3, 13, 8 });
    }

    SECTION("relative")
    {
        loader.RunString(R"(
Lot-->
   extrude(10)
   t('0.5,'1,'1.5)
)", *eval/*, true*/);

        geos = eval->Eval(geos, ctx);

        REQUIRE(geos.size() == 1);
        test::check_aabb(*geos[0], { 1, 10, 4.5f }, { 3, 20, 7.5f });
    }
}