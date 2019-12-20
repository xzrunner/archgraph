#include "utility.h"

#include <cga/node/Color.h>

#include <cga/node/PrimQuad.h>

#include <cga/Evaluator.h>
#include <cga/Geometry.h>

#include <catch/catch.hpp>

TEST_CASE("color")
{
    test::init();

    cga::Evaluator eval;

    auto quad = std::make_shared<cga::node::PrimQuad>();
    eval.AddNode(quad);

    auto color = std::make_shared<cga::node::Color>();
    color->SetColor({ 1, 0.5f, 0 });
    eval.AddNode(color);

    eval.Connect({ quad, 0 }, { color, 0 });

    auto geos = eval.Eval();

    auto geo = test::query_geo(geos, color);
    auto& col = geo->GetColor();
    REQUIRE(col.x == 1);
    REQUIRE(col.y == 0.5f);
    REQUIRE(col.z == 0);
}