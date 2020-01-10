#include "utility.h"

#include <cga/EvalNode.h>
#include <cga/RuleLoader.h>
#include <cga/EvalRule.h>
#include <cga/Geometry.h>

#include <cga/node/PrimCube.h>

#include <catch/catch.hpp>

TEST_CASE("scope_attribute")
{
    test::init();

    cga::EvalContext ctx;

    cga::RuleLoader loader;
    auto eval = std::make_shared<cga::EvalRule>();

    std::vector<cga::GeoPtr> _geos, geos;
    auto cube = std::make_shared<cga::node::PrimCube>();
    cube->SetWidth(1);
    cube->SetHeight(2);
    cube->SetDepth(3);
    cube->Execute(_geos, geos, ctx);
    assert(geos.size() == 1);

    SECTION("size")
    {
        loader.RunString(R"(
Wall --> print(scope.sx)
         s(10, '1, '1)
         print(scope.sx)
)", *eval/*, true*/);

        geos = eval->Eval(geos);

    }
}