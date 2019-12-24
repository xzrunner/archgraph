#include "utility.h"

#include <cga/node/Comp.h>
#include <cga/node/Offset.h>
#include <cga/node/ShapeO.h>
#include <cga/node/Split.h>

#include <cga/node/PrimQuad.h>
#include <cga/node/PrimCube.h>

#include <cga/Evaluator.h>
#include <cga/Geometry.h>
#include <cga/RuleLoader.h>

#include <catch/catch.hpp>

TEST_CASE("comp")
{
    test::init();

    cga::Evaluator eval;

    auto cube = std::make_shared<cga::node::PrimCube>();
    eval.AddNode(cube);

    auto comp = std::make_shared<cga::node::Comp>();
    eval.AddNode(comp);

    eval.Connect({ cube, 0 }, { comp, 0 });

    SECTION("face6")
    {
        comp->SetSelector({
            cga::node::Comp::Selector::Front,
            cga::node::Comp::Selector::Back,
            cga::node::Comp::Selector::Left,
            cga::node::Comp::Selector::Right,
            cga::node::Comp::Selector::Top,
            cga::node::Comp::Selector::Bottom
        });

        auto geos = eval.Eval();
        auto front  = test::query_geo(geos, comp, 0);
        auto back   = test::query_geo(geos, comp, 1);
        auto left   = test::query_geo(geos, comp, 2);
        auto right  = test::query_geo(geos, comp, 3);
        auto top    = test::query_geo(geos, comp, 4);
        auto bottom = test::query_geo(geos, comp, 5);
        test::check_single_face_norm(*front,  sm::vec3(0, 0, -1));
        test::check_single_face_norm(*back,   sm::vec3(0, 0,  1));
        test::check_single_face_norm(*left,   sm::vec3(-1, 0, 0));
        test::check_single_face_norm(*right,  sm::vec3( 1, 0, 0));
        test::check_single_face_norm(*top,    sm::vec3(0,  1, 0));
        test::check_single_face_norm(*bottom, sm::vec3(0, -1, 0));
    }

    SECTION("normal y")
    {
        comp->SetSelector({
            cga::node::Comp::Selector::Vertical,
            cga::node::Comp::Selector::Horizontal,
            cga::node::Comp::Selector::Aslant,
            cga::node::Comp::Selector::Nutant
        });

        auto geos = eval.Eval();
        auto vert   = test::query_geo(geos, comp, 0);
        auto hori   = test::query_geo(geos, comp, 1);
        auto aslant = test::query_geo(geos, comp, 2);
        auto nutant = test::query_geo(geos, comp, 3);
        test::check_faces_num(*vert, 4);
        test::check_faces_num(*hori, 2);
        REQUIRE(!aslant);
        REQUIRE(!nutant);
    }
}

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

//TEST_CASE("split rule")
//{
//    test::init();
//
//    cga::RuleLoader loader;
//    auto eval = loader.RunString(R"(
//X(h)-->
//   s('1,'h,'1)
//   color("#0000ff")
//
//Y(h)-->
//   s('1,'h,'1)
//   color("#ffff00")
//
//Z(h)-->
//   s('1,'h,'1)
//   color("#00ff00")
//
//A-->
//   split(x){ '0.5 : Z
//           | '0.1 : Y(2)
//           | '0.2 : X(1) }
//
//)", true);
//
//    int zz = 0;
//}