#include "utility.h"

#include <ce/op/PrimQuad.h>
#include <ce/op/Extrude.h>
#include <ce/op/Color.h>

#include <ce/EvalOp.h>
#include <ce/EvalRule.h>

#include <catch/catch.hpp>

TEST_CASE("nodes to rule")
{
    test::init();

    ce::EvalContext ctx;

    ce::EvalOp eval;

    auto extrude = std::make_shared<ce::op::Extrude>();
    extrude->SetDistance(1);
    eval.AddOp(extrude);

    auto color = std::make_shared<ce::op::Color>();
    color->SetColor({ 1, 0, 0 });
    eval.AddOp(color);

    eval.Connect({ extrude, 0 }, { color, 0 });

    auto rule_eval = eval.ToRule(ctx);

    SECTION("quad")
    {
        auto quad = std::make_shared<ce::op::PrimQuad>();
        quad->SetLength(1);
        quad->SetWidth(2);

        std::vector<ce::GeoPtr> geos;
        quad->Execute(std::vector<ce::GeoPtr>(), geos, ce::EvalContext());

        auto dst_geos = rule_eval->Eval(geos, ctx);

        REQUIRE(dst_geos.size() == 1);
        test::check_points_num(*dst_geos[0], 8);
        test::check_faces_num(*dst_geos[0], 6);
        test::check_aabb(*dst_geos[0], { 0, 0, 0 }, { 2, 1, 1 });
        test::check_color(*dst_geos[0], { 1, 0, 0 });
    }
}