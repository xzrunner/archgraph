#include "utility.h"

#include <cga/op/Color.h>

#include <cga/op/PrimQuad.h>

#include <cga/EvalOp.h>
#include <cga/Geometry.h>

#include <catch/catch.hpp>

TEST_CASE("color")
{
    test::init();

    cga::EvalOp eval;

    auto quad = std::make_shared<cga::op::PrimQuad>();
    eval.AddOp(quad);

    auto color = std::make_shared<cga::op::Color>();
    color->SetColor({ 1, 0.5f, 0 });
    eval.AddOp(color);

    eval.Connect({ quad, 0 }, { color, 0 });

    auto geos = eval.Eval();

    auto geo = test::query_geo(geos, color);
    test::check_color(*geo, { 1, 0.5f, 0 });
}