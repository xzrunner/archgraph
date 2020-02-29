#include "utility.h"

#include <archgraph/EvalOp.h>
#include <archgraph/RuleLoader.h>
#include <archgraph/EvalRule.h>
#include <archgraph/Geometry.h>

#include <archgraph/op/PrimCube.h>
#include <archgraph/op/PrimQuad.h>

#include <catch/catch.hpp>

TEST_CASE("scope_attr")
{
    test::init();

    archgraph::EvalContext ctx;

    archgraph::RuleLoader loader;

    auto eval = std::make_shared<archgraph::EvalRule>();

    std::vector<archgraph::GeoPtr> _geos, geos;
    auto cube = std::make_shared<archgraph::op::PrimCube>();
    cube->SetWidth(1);
    cube->SetHeight(2);
    cube->SetDepth(3);
    cube->Execute(_geos, geos, ctx);
    assert(geos.size() == 1);

    SECTION("size")
    {
        loader.RunString(ctx, R"(
Wall --> print(scope.sx)
         s(10, '1, '1)
         print(scope.sx)
)", *eval/*, true*/);

        geos = eval->Eval(geos, ctx);
        auto& consel = eval->GetConsole();
        REQUIRE(consel.str() == "1\n10\n");
    }
}

TEST_CASE("split_attr")
{
    test::init();

    archgraph::EvalContext ctx;

    archgraph::RuleLoader loader;

    auto eval = std::make_shared<archgraph::EvalRule>();

    std::vector<archgraph::GeoPtr> _geos, geos;
    auto cube = std::make_shared<archgraph::op::PrimQuad>();
    cube->Execute(_geos, geos, ctx);
    assert(geos.size() == 1);

    loader.RunString(ctx, R"(
Lot -->	extrude(20) A

A --> 	split(y){2 : B}*

B -->	case (split.index == 0) :
     		color("#ff0000") X
     	case (split.index == split.total-1) :
     		color("#0000ff") X
     	else:
     		color("#ffffff") X

X -->   print("todo")

)", *eval/*, true*/);

    geos = eval->Eval(geos, ctx);
    REQUIRE(geos.size() == 10);
    test::check_color(*geos[0], { 1, 0, 0 });
    test::check_color(*geos[1], { 1, 1, 1 });
    test::check_color(*geos[9], { 0, 0, 1 });
    test::check_aabb(*geos[5], { 0, 10, 0 }, { 1, 12, 1 });
}