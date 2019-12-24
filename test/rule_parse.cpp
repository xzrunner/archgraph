#include "utility.h"

#include <cga/node/Color.h>
#include <cga/node/PrimQuad.h>

#include <cga/Evaluator.h>
#include <cga/RuleLoader.h>
#include <cga/EvalRule.h>
#include <cga/Geometry.h>

#include <catch/catch.hpp>

TEST_CASE("rule simple")
{
    test::init();

    cga::RuleLoader loader;
    auto eval = loader.RunString(R"(
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
)"/*, true*/);

    std::vector<cga::GeoPtr> _geos, geos;
    auto quad = std::make_shared<cga::node::PrimQuad>();
    quad->Execute(_geos, geos);
    assert(geos.size() == 1);

    geos = eval->Eval(geos);

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
//    cga::Evaluator eval;
//
//    cga::RuleLoader loader;
//    loader.RunString(R"(
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
