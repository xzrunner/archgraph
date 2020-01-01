#include "utility.h"

#include <cga/node/Color.h>

#include <cga/node/PrimQuad.h>

#include <cga/EvalNode.h>
#include <cga/Geometry.h>

#include <catch/catch.hpp>

TEST_CASE("color")
{
    test::init();

    cga::EvalNode eval;

    auto quad = std::make_shared<cga::node::PrimQuad>();
    eval.AddNode(quad);

    auto color = std::make_shared<cga::node::Color>();
    color->SetColor({ 1, 0.5f, 0 });
    eval.AddNode(color);

    eval.Connect({ quad, 0 }, { color, 0 });

    auto geos = eval.Eval();

    auto geo = test::query_geo(geos, color);
    test::check_color(*geo, { 1, 0.5f, 0 });
}