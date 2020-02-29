#include "utility.h"

#include <archgraph/op/PrimQuad.h>
#include <archgraph/op/Extrude.h>
#include <archgraph/op/Color.h>

#include <archgraph/EvalOp.h>
#include <archgraph/EvalRule.h>

#include <catch/catch.hpp>

TEST_CASE("nodes to rule")
{
    test::init();

    archgraph::EvalContext ctx;

    archgraph::EvalOp eval;

    auto extrude = std::make_shared<archgraph::op::Extrude>();
    extrude->SetDistance(1);
    eval.AddOp(extrude);

    auto color = std::make_shared<archgraph::op::Color>();
    color->SetColor({ 1, 0, 0 });
    eval.AddOp(color);

    eval.Connect({ extrude, 0 }, { color, 0 });

    auto rule_eval = eval.ToRule(ctx);

    SECTION("quad")
    {
        auto quad = std::make_shared<archgraph::op::PrimQuad>();
        quad->SetLength(1);
        quad->SetWidth(2);

        std::vector<archgraph::GeoPtr> geos;
        quad->Execute(std::vector<archgraph::GeoPtr>(), geos, archgraph::EvalContext());

        auto dst_geos = rule_eval->Eval(geos, ctx);

        REQUIRE(dst_geos.size() == 1);
        test::check_points_num(*dst_geos[0], 8);
        test::check_faces_num(*dst_geos[0], 6);
        test::check_aabb(*dst_geos[0], { 0, 0, 0 }, { 2, 1, 1 });
        test::check_color(*dst_geos[0], { 1, 0, 0 });
    }
}