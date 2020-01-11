#include "utility.h"

#include <cga/node/Comp.h>
#include <cga/node/Offset.h>
#include <cga/node/ShapeO.h>
#include <cga/node/Split.h>

#include <cga/node/PrimQuad.h>
#include <cga/node/PrimCube.h>

#include <cga/EvalNode.h>
#include <cga/Geometry.h>
#include <cga/RuleLoader.h>
#include <cga/EvalRule.h>

#include <catch/catch.hpp>

#include <sstream>

TEST_CASE("comp")
{
    test::init();

    cga::EvalNode eval;

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

    cga::EvalNode eval;

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
#ifdef BUILD_CENTER
        test::check_aabb(*geo, { -0.5f, 0, -1.5f }, { 0.5f, 0, 1.5f });
#else
        test::check_aabb(*geo, { 0.5f, 0, 0.5f }, { 1.5f, 0, 3.5f });
#endif // BUILD_CENTER
    }

    SECTION("shrink + border")
    {
        offset->SetDistance(-0.5f);
        offset->SetSelector(cga::node::Offset::Selector::Border);

        auto geos = eval.Eval();

        auto geo = test::query_geo(geos, offset);
        test::check_points_num(*geo, 8);
        test::check_faces_num(*geo, 1);
#ifdef BUILD_CENTER
        test::check_aabb(*geo, { -1, 0, -2 }, { 1, 0, 2 });
        test::check_aabb_holes(*geo, { -0.5f, 0, -1.5f }, { 0.5f, 0, 1.5f });
#else
        test::check_aabb(*geo, { 0, 0, 0 }, { 2, 0, 4 });
        test::check_aabb_holes(*geo, { 0.5f, 0, 0.5f }, { 1.5f, 0, 3.5f });
#endif // BUILD_CENTER
    }

    SECTION("shrink + all")
    {
        offset->SetDistance(-0.5f);
        offset->SetSelector(cga::node::Offset::Selector::All);

        auto geos = eval.Eval();

        auto geo = test::query_geo(geos, offset);
        test::check_points_num(*geo, 8);
        test::check_faces_num(*geo, 2);
#ifdef BUILD_CENTER
        test::check_aabb(*geo, { -1, 0, -2 }, { 1, 0, 2 });
        test::check_aabb_holes(*geo, { -0.5f, 0, -1.5f }, { 0.5f, 0, 1.5f });
#else
        test::check_aabb(*geo, { 0, 0, 0 }, { 2, 0, 4 });
        test::check_aabb_holes(*geo, { 0.5f, 0, 0.5f }, { 1.5f, 0, 3.5f });
#endif // BUILD_CENTER
    }

    SECTION("enlarge + inside")
    {
        offset->SetDistance(0.5f);
        offset->SetSelector(cga::node::Offset::Selector::Inside);

        auto geos = eval.Eval();

        auto geo = test::query_geo(geos, offset);
        test::check_points_num(*geo, 4);
        test::check_faces_num(*geo, 1);
#ifdef BUILD_CENTER
        test::check_aabb(*geo, { -1, 0, -2 }, { 1, 0, 2 });
#else
        test::check_aabb(*geo, { 0, 0, 0 }, { 2, 0, 4 });
#endif // BUILD_CENTER
    }

    SECTION("enlarge + border")
    {
        offset->SetDistance(0.5f);
        offset->SetSelector(cga::node::Offset::Selector::Border);

        auto geos = eval.Eval();

        auto geo = test::query_geo(geos, offset);
        test::check_points_num(*geo, 8);
        test::check_faces_num(*geo, 1);
#ifdef BUILD_CENTER
        test::check_aabb(*geo, { -1.5f, 0, -2.5f }, { 1.5f, 0, 2.5f });
        test::check_aabb_holes(*geo, { -1, 0, -2 }, { 1, 0, 2 });
#else
        test::check_aabb(*geo, { -0.5f, 0, -0.5f }, { 2.5f, 0, 4.5f });
        test::check_aabb_holes(*geo, { 0, 0, 0 }, { 2, 0, 4 });
#endif // BUILD_CENTER
    }

    SECTION("enlarge + all")
    {
        offset->SetDistance(0.5f);
        offset->SetSelector(cga::node::Offset::Selector::All);

        auto geos = eval.Eval();

        auto geo = test::query_geo(geos, offset);
        test::check_points_num(*geo, 8);
        test::check_faces_num(*geo, 2);
#ifdef BUILD_CENTER
        test::check_aabb(*geo, { -1.5f, 0, -2.5f }, { 1.5f, 0, 2.5f });
        test::check_aabb_holes(*geo, { -1, 0, -2 }, { 1, 0, 2 });
#else
        test::check_aabb(*geo, { -0.5f, 0, -0.5f }, { 2.5f, 0, 4.5f });
        test::check_aabb_holes(*geo, { 0, 0, 0 }, { 2, 0, 4 });
#endif // BUILD_CENTER
    }
}

TEST_CASE("shapeo")
{
    test::init();

    cga::EvalNode eval;

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
#ifdef BUILD_CENTER
    test::check_aabb(*geo, { -1, 0, -2 }, { 1, 0, 2 });
    test::check_aabb_holes(*geo, { -0.5f, 0, -1.5f }, { 0.5f, 0, 1.5f });
#else
    test::check_aabb(*geo, { 0, 0, 0 }, { 2, 0, 4 });
    test::check_aabb_holes(*geo, { 0.5f, 0, 0.5f }, { 1.5f, 0, 3.5f });
#endif // BUILD_CENTER
}

TEST_CASE("split")
{
    test::init();

    cga::EvalNode eval;

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
            { cga::node::Split::SizeType::Relative, 0.2f },
            { cga::node::Split::SizeType::Relative, 0.8f }
        });

        auto geos = eval.Eval();
        REQUIRE(geos.size() == 2);
        auto geo0 = test::query_geo(geos, split, 0);
        auto geo1 = test::query_geo(geos, split, 1);
        test::check_single_face_area(*geo0, 2);
        test::check_single_face_area(*geo1, 8);
    }
}

TEST_CASE("split rule")
{
    test::init();

    cga::EvalContext ctx;

    cga::RuleLoader loader;

    std::stringstream ss;
    auto eval = std::make_shared<cga::EvalRule>(ss);

    // Setup
    loader.RunString(R"(
X(h)-->
   s('1,'h,'1)
   color("#0000ff")

Y(h)-->
   s('1,'h,'1)
   color("#ffff00")

Z(h)-->
   s('1,'h,'1)
   color("#00ff00")
)", *eval/*, true*/);

    std::vector<cga::GeoPtr> _geos, init_geos;
    auto quad = std::make_shared<cga::node::PrimCube>();
    quad->SetWidth(10);
    quad->SetHeight(1);
    quad->SetDepth(1);
    quad->Execute(_geos, init_geos, ctx);
    assert(init_geos.size() == 1);

    SECTION("Relative Sizes")
    {
        loader.RunString(R"(
A-->
   split(x){ '0.5 : Z(1)
           | '0.1 : Y(2)
           | '0.2 : X(1) }
    )", *eval/*, true*/);

        auto rule_a = eval->QueryRule("A");
        REQUIRE(rule_a != nullptr);
        auto a_ops = rule_a->GetAllOps();
        REQUIRE(a_ops.size() == 1);
        REQUIRE(a_ops[0]->selectors.sels.size() == 3);

        auto geos = eval->Eval(init_geos);

        REQUIRE(geos.size() == 3);
        test::check_color(*geos[0], sm::vec3(0, 1, 0));
        test::check_color(*geos[1], sm::vec3(1, 1, 0));
        test::check_color(*geos[2], sm::vec3(0, 0, 1));
        test::check_aabb(*geos[0], sm::vec3(0, 0, 0), sm::vec3(5, 1, 1));
        test::check_aabb(*geos[1], sm::vec3(5, 0, 0), sm::vec3(6, 2, 1));
        test::check_aabb(*geos[2], sm::vec3(6, 0, 0), sm::vec3(8, 1, 1));
    }

    SECTION("Floating Sizes only: Ratios")
    {
        loader.RunString(R"(
A-->
   split(x){ ~0.5 : Z(1)
           | ~0.1 : Y(2)
           | ~0.2 : X(1) }
    )", *eval/*, true*/);

        auto rule_a = eval->QueryRule("A");
        REQUIRE(rule_a != nullptr);
        auto a_ops = rule_a->GetAllOps();
        REQUIRE(a_ops.size() == 1);
        REQUIRE(a_ops[0]->selectors.sels.size() == 3);

        auto geos = eval->Eval(init_geos);

        REQUIRE(geos.size() == 3);
        test::check_color(*geos[0], sm::vec3(0, 1, 0));
        test::check_color(*geos[1], sm::vec3(1, 1, 0));
        test::check_color(*geos[2], sm::vec3(0, 0, 1));
        test::check_aabb(*geos[0], sm::vec3(0,     0, 0), sm::vec3(6.25f, 1, 1));
        test::check_aabb(*geos[1], sm::vec3(6.25f, 0, 0), sm::vec3(7.5f,  2, 1));
        test::check_aabb(*geos[2], sm::vec3(7.5f,  0, 0), sm::vec3(10,    1, 1));
    }

    SECTION("Oversized")
    {
        loader.RunString(R"(
A-->
   split(x){ '0.5 : Z(1)
           | '0.6 : Y(2)
           | 3    : X(1) }
    )", *eval/*, true*/);

        auto rule_a = eval->QueryRule("A");
        REQUIRE(rule_a != nullptr);
        auto a_ops = rule_a->GetAllOps();
        REQUIRE(a_ops.size() == 1);
        REQUIRE(a_ops[0]->selectors.sels.size() == 3);

        auto geos = eval->Eval(init_geos);

        REQUIRE(geos.size() == 3);
        test::check_color(*geos[0], sm::vec3(0, 1, 0));
        test::check_color(*geos[1], sm::vec3(1, 1, 0));
        test::check_aabb(*geos[0], sm::vec3(0, 0, 0), sm::vec3(5,  1, 1));
        test::check_aabb(*geos[1], sm::vec3(5, 0, 0), sm::vec3(10, 2, 1));
        REQUIRE(geos[2] == nullptr);
    }

    SECTION("Repeat Split with Absolute Sizes")
    {
        loader.RunString(R"(
A-->
   split(x){ 2 : X(2)
           | 1 : Y(1) }*
    )", *eval/*, true*/);

        auto rule_a = eval->QueryRule("A");
        REQUIRE(rule_a != nullptr);
        auto a_ops = rule_a->GetAllOps();
        REQUIRE(a_ops.size() == 1);
        REQUIRE(a_ops[0]->selectors.sels.size() == 2);
        REQUIRE(a_ops[0]->selectors.duplicate);

        auto geos = eval->Eval(init_geos);

        REQUIRE(geos.size() == 8);
        REQUIRE(geos[7] == nullptr);
        test::check_color(*geos[0], sm::vec3(0, 0, 1));
        test::check_color(*geos[1], sm::vec3(1, 1, 0));
        test::check_color(*geos[2], sm::vec3(0, 0, 1));
        test::check_color(*geos[3], sm::vec3(1, 1, 0));
        test::check_color(*geos[4], sm::vec3(0, 0, 1));
        test::check_color(*geos[5], sm::vec3(1, 1, 0));
        test::check_color(*geos[6], sm::vec3(0, 0, 1));
        test::check_aabb(*geos[0], sm::vec3(0, 0, 0), sm::vec3(2, 2, 1));
        test::check_aabb(*geos[1], sm::vec3(2, 0, 0), sm::vec3(3, 1, 1));
        test::check_aabb(*geos[2], sm::vec3(3, 0, 0), sm::vec3(5, 2, 1));
        test::check_aabb(*geos[3], sm::vec3(5, 0, 0), sm::vec3(6, 1, 1));
        test::check_aabb(*geos[4], sm::vec3(6, 0, 0), sm::vec3(8, 2, 1));
        test::check_aabb(*geos[5], sm::vec3(8, 0, 0), sm::vec3(9, 1, 1));
        test::check_aabb(*geos[6], sm::vec3(9, 0, 0), sm::vec3(10, 2, 1));
    }

    SECTION("Repeat Split with Floating Sizes")
    {
        loader.RunString(R"(
A-->
   split(x){ ~2 : X(2) |
             ~1 : Y(1) }*
    )", *eval/*, true*/);

        auto rule_a = eval->QueryRule("A");
        REQUIRE(rule_a != nullptr);
        auto a_ops = rule_a->GetAllOps();
        REQUIRE(a_ops.size() == 1);
        REQUIRE(a_ops[0]->selectors.sels.size() == 2);
        REQUIRE(a_ops[0]->selectors.duplicate);

        auto geos = eval->Eval(init_geos);

        REQUIRE(geos.size() == 6);
        test::check_color(*geos[0], sm::vec3(0, 0, 1));
        test::check_color(*geos[1], sm::vec3(1, 1, 0));
        test::check_color(*geos[2], sm::vec3(0, 0, 1));
        test::check_color(*geos[3], sm::vec3(1, 1, 0));
        test::check_color(*geos[4], sm::vec3(0, 0, 1));
        test::check_color(*geos[5], sm::vec3(1, 1, 0));
        test::check_aabb(*geos[0], sm::vec3(0, 0, 0),           sm::vec3(2.0f / 0.9f, 2, 1));
        test::check_aabb(*geos[1], sm::vec3(2.0f / 0.9f, 0, 0), sm::vec3(3.0f / 0.9f, 1, 1));
        test::check_aabb(*geos[2], sm::vec3(3.0f / 0.9f, 0, 0), sm::vec3(5.0f / 0.9f, 2, 1));
        test::check_aabb(*geos[3], sm::vec3(5.0f / 0.9f, 0, 0), sm::vec3(6.0f / 0.9f, 1, 1));
        test::check_aabb(*geos[4], sm::vec3(6.0f / 0.9f, 0, 0), sm::vec3(8.0f / 0.9f, 2, 1));
        test::check_aabb(*geos[5], sm::vec3(8.0f / 0.9f, 0, 0), sm::vec3(10, 1, 1));
    }

    SECTION("Interleaved Repeat Split")
    {
        loader.RunString(R"(
A-->
   split(x) { 1 : X(3)
            | {  ~1 : Y(2)
              | 0.2 : Z(1)
              | ~1  : Y(2) }*
            | 1 : X(3) }
    )", *eval/*, true*/);

        auto rule_a = eval->QueryRule("A");
        REQUIRE(rule_a != nullptr);
        auto a_ops = rule_a->GetAllOps();
        REQUIRE(a_ops.size() == 1);
        REQUIRE(a_ops[0]->selectors.sels.size() == 3);
        REQUIRE(a_ops[0]->selectors.sels[1]->duplicate);

        auto geos = eval->Eval(init_geos);

        REQUIRE(geos.size() == 14);
        test::check_color(*geos[0], sm::vec3(0, 0, 1));
        test::check_color(*geos[1], sm::vec3(1, 1, 0));
        test::check_color(*geos[2], sm::vec3(0, 1, 0));
        test::check_color(*geos[3], sm::vec3(1, 1, 0));
        test::check_color(*geos[4], sm::vec3(1, 1, 0));
        test::check_color(*geos[5], sm::vec3(0, 1, 0));
        test::check_color(*geos[6], sm::vec3(1, 1, 0));
        test::check_color(*geos[13], sm::vec3(0, 0, 1));
        test::check_aabb(*geos[0], sm::vec3(0.0f, 0, 0), sm::vec3(1.0f, 3, 1));
        test::check_aabb(*geos[1], sm::vec3(1.0f, 0, 0), sm::vec3(1.9f, 2, 1));
        test::check_aabb(*geos[2], sm::vec3(1.9f, 0, 0), sm::vec3(2.1f, 1, 1));
        test::check_aabb(*geos[3], sm::vec3(2.1f, 0, 0), sm::vec3(3.0f, 2, 1));
        test::check_aabb(*geos[4], sm::vec3(3.0f, 0, 0), sm::vec3(3.9f, 2, 1));
        test::check_aabb(*geos[5], sm::vec3(3.9f, 0, 0), sm::vec3(4.1f, 1, 1));
        test::check_aabb(*geos[6], sm::vec3(4.1f, 0, 0), sm::vec3(5.0f, 2, 1));
        test::check_aabb(*geos[13], sm::vec3(9, 0, 0), sm::vec3(10, 3, 1));
    }

    SECTION("Rhythm")
    {
        loader.RunString(R"(
A-->
   split(x) { { 1  : X(3)
              | ~3 : Y(1) }*
            | 1 : X(3) }
    )", *eval/*, true*/);

        auto rule_a = eval->QueryRule("A");
        REQUIRE(rule_a != nullptr);
        auto a_ops = rule_a->GetAllOps();
        REQUIRE(a_ops.size() == 1);
        REQUIRE(a_ops[0]->selectors.sels.size() == 2);
        REQUIRE(a_ops[0]->selectors.sels[0]->duplicate);

        auto geos = eval->Eval(init_geos);

        REQUIRE(geos.size() == 7);
        test::check_color(*geos[0], sm::vec3(0, 0, 1));
        test::check_color(*geos[1], sm::vec3(1, 1, 0));
        test::check_color(*geos[2], sm::vec3(0, 0, 1));
        test::check_color(*geos[3], sm::vec3(1, 1, 0));
        test::check_color(*geos[4], sm::vec3(0, 0, 1));
        test::check_color(*geos[5], sm::vec3(1, 1, 0));
        test::check_color(*geos[6], sm::vec3(0, 0, 1));
        test::check_aabb(*geos[0], sm::vec3(0, 0, 0), sm::vec3(1, 3, 1));
        test::check_aabb(*geos[1], sm::vec3(1, 0, 0), sm::vec3(3, 1, 1));
        test::check_aabb(*geos[2], sm::vec3(3, 0, 0), sm::vec3(4, 3, 1));
        test::check_aabb(*geos[3], sm::vec3(4, 0, 0), sm::vec3(6, 1, 1));
        test::check_aabb(*geos[4], sm::vec3(6, 0, 0), sm::vec3(7, 3, 1));
        test::check_aabb(*geos[5], sm::vec3(7, 0, 0), sm::vec3(9, 1, 1));
        test::check_aabb(*geos[6], sm::vec3(9, 0, 0), sm::vec3(10, 3, 1));
    }
}