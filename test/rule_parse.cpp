#include "utility.h"

#include <archgraph/op/Color.h>
#include <archgraph/op/PrimQuad.h>

#include <archgraph/EvalOp.h>
#include <archgraph/RuleLoader.h>
#include <archgraph/EvalRule.h>
#include <archgraph/Geometry.h>

#include <catch/catch.hpp>

#include <sstream>

TEST_CASE("rule simple")
{
    test::init();

    archgraph::EvalContext ctx;

    archgraph::RuleLoader loader;

    auto eval = std::make_shared<archgraph::EvalRule>();

    loader.RunString(ctx, R"(
attr red   = "#FF0000"
attr green = "#00FF00"

Lot -->
   offset(-3) A

A -->
   comp(f) { inside : I
           | border : O }

I -->
   color(red)

O -->
   color(green)
)", *eval/*, true*/);

    std::vector<archgraph::GeoPtr> _geos, geos;
    auto quad = std::make_shared<archgraph::op::PrimQuad>();
    quad->Execute(_geos, geos, ctx);
    assert(geos.size() == 1);

    geos = eval->Eval(geos, ctx);

    REQUIRE(geos.size() == 2);
    REQUIRE(geos[0]->GetColor() == sm::vec3(1, 0, 0));
    REQUIRE(geos[1]->GetColor() == sm::vec3(0, 1, 0));
    REQUIRE(geos[0]->GetPoly()->Points().size() == 4);
    REQUIRE(geos[0]->GetPoly()->Faces().size() == 1);
    REQUIRE(geos[0]->GetPoly()->Faces()[0]->holes.empty());
    REQUIRE(geos[1]->GetPoly()->Points().size() == 8);
    REQUIRE(geos[1]->GetPoly()->Faces().size() == 1);
    REQUIRE(geos[1]->GetPoly()->Faces()[0]->holes.size() == 1);
}

//TEST_CASE("rule parms and comment")
//{
//    test::init();
//
//    archgraph::EvalOp eval;
//
//    archgraph::RuleLoader loader;
//    loader.RunString(ctx, R"(
//Facade (n) --> # horizontal splits and pediments
//	split(y) {
//		groundFloorHeight : Ground(n) |
//		bigRailHeight : s(scope.sx+5, '1, '1) t(-2.5,0,0) extrude(0.1) trim() Wall |
//		~3.0 : One(n) |
//		bigRailHeight : s(scope.sx+5, '1, '1) t(-2.5,0,0) extrude(0.1) trim() Wall |
//		midRailOffset : Three(n) |
//		1.4: s(scope.sx + 5,'1, 0.3) t(-2.5,0,0) i("assets/6365_top_ledge.obj") Wall |
//		2: Four(n)
//	}
//)", eval, true);
//}
