#include "utility.h"

#include <cga/node/Offset.h>
#include <cga/node/ShapeO.h>

#include <cga/node/PrimQuad.h>

#include <cga/Evaluator.h>

#include <catch/catch.hpp>

TEST_CASE("offset")
{
    test::init();

    cga::Evaluator eval;

    auto quad = std::make_shared<cga::node::PrimQuad>();
    quad->SetWidth(2);
    quad->SetLength(4);
    eval.AddNode(quad);

    auto offset = std::make_shared<cga::node::Offset>();
    eval.AddNode(offset);

    eval.Connect({ quad, 0 }, { offset, 0 });

    SECTION("shrink + inside")
    {
        offset->SetDistance(-0.5f);
        offset->SetSelector(cga::node::Offset::Selector::Inside);

        auto geos = eval.Eval();
        auto geo = test::query_geo(geos, offset);
        test::check_points_num(*geo, 4);
        test::check_faces_num(*geo, 1);
        test::check_aabb(*geo, { -0.5f, 0, -1.5f }, { 0.5f, 0, 1.5f });
    }

    SECTION("shrink + border")
    {
        offset->SetDistance(-0.5f);
        offset->SetSelector(cga::node::Offset::Selector::Border);

        auto geos = eval.Eval();

        auto geo = test::query_geo(geos, offset);
        test::check_points_num(*geo, 8);
        test::check_faces_num(*geo, 1);
        test::check_aabb(*geo, { -1, 0, -2 }, { 1, 0, 2 });
        test::check_aabb_holes(*geo, { -0.5f, 0, -1.5f }, { 0.5f, 0, 1.5f });
    }

    SECTION("shrink + all")
    {
        offset->SetDistance(-0.5f);
        offset->SetSelector(cga::node::Offset::Selector::All);

        auto geos = eval.Eval();

        auto geo = test::query_geo(geos, offset);
        test::check_points_num(*geo, 8);
        test::check_faces_num(*geo, 2);
        test::check_aabb(*geo, { -1, 0, -2 }, { 1, 0, 2 });
        test::check_aabb_holes(*geo, { -0.5f, 0, -1.5f }, { 0.5f, 0, 1.5f });
    }

    SECTION("enlarge + inside")
    {
        offset->SetDistance(0.5f);
        offset->SetSelector(cga::node::Offset::Selector::Inside);

        auto geos = eval.Eval();

        auto geo = test::query_geo(geos, offset);
        test::check_points_num(*geo, 4);
        test::check_faces_num(*geo, 1);
        test::check_aabb(*geo, { -1, 0, -2 }, { 1, 0, 2 });
    }

    SECTION("enlarge + border")
    {
        offset->SetDistance(0.5f);
        offset->SetSelector(cga::node::Offset::Selector::Border);

        auto geos = eval.Eval();

        auto geo = test::query_geo(geos, offset);
        test::check_points_num(*geo, 8);
        test::check_faces_num(*geo, 1);
        test::check_aabb(*geo, { -1.5f, 0, -2.5f }, { 1.5f, 0, 2.5f });
        test::check_aabb_holes(*geo, { -1, 0, -2 }, { 1, 0, 2 });
    }

    SECTION("enlarge + all")
    {
        offset->SetDistance(0.5f);
        offset->SetSelector(cga::node::Offset::Selector::All);

        auto geos = eval.Eval();

        auto geo = test::query_geo(geos, offset);
        test::check_points_num(*geo, 8);
        test::check_faces_num(*geo, 2);
        test::check_aabb(*geo, { -1.5f, 0, -2.5f }, { 1.5f, 0, 2.5f });
        test::check_aabb_holes(*geo, { -1, 0, -2 }, { 1, 0, 2 });
    }
}

TEST_CASE("shapeo")
{
    test::init();

    cga::Evaluator eval;

    auto quad = std::make_shared<cga::node::PrimQuad>();
    quad->SetWidth(2);
    quad->SetLength(4);
    eval.AddNode(quad);

    auto shapeo = std::make_shared<cga::node::ShapeO>();
    shapeo->SetFrontWidth(0.5f);
    eval.AddNode(shapeo);

    eval.Connect({ quad, 0 }, { shapeo, 0 });

    auto geos = eval.Eval();

    auto geo = test::query_geo(geos, shapeo, cga::node::ShapeO::OUT_SHAPE);
    test::check_points_num(*geo, 8);
    test::check_faces_num(*geo, 1);
    test::check_aabb(*geo, { -1, 0, -2 }, { 1, 0, 2 });
    test::check_aabb_holes(*geo, { -0.5f, 0, -1.5f }, { 0.5f, 0, 1.5f });
}
TEST_CASE("split")
{
    test::init();

    cga::Evaluator eval;

    auto quad = std::make_shared<cga::node::PrimQuad>();
    quad->SetWidth(10);
    quad->SetLength(1);
    eval.AddNode(quad);

    auto split = std::make_shared<cga::node::Split>();
    split->SetAxis(cga::node::Split::Axis::X);
    eval.AddNode(split);

    eval.Connect({ quad, 0 }, { split, 0 });

    SECTION("absolute0")
    {
        split->SetParts({
            { cga::node::Split::SizeType::Absolute, 0.2f },
            { cga::node::Split::SizeType::Absolute, 0.8f }
        });

        auto geos = eval.Eval();
        REQUIRE(geos.size() == 2);
        auto geo0 = test::query_geo(geos, split, 0);
        auto geo1 = test::query_geo(geos, split, 1);
        test::check_single_face_area(*geo0, 2);
        test::check_single_face_area(*geo1, 8);
    }
}
