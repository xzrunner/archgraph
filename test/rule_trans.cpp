#include "utility.h"

#include <cga/node/PrimQuad.h>
#include <cga/node/Extrude.h>
#include <cga/node/Color.h>

#include <cga/EvalNode.h>
#include <cga/EvalRule.h>

#include <catch/catch.hpp>

TEST_CASE("nodes to rule")
{
    test::init();

    cga::EvalNode eval;

    auto extrude = std::make_shared<cga::node::Extrude>();
    extrude->SetDistance(1);
    eval.AddNode(extrude);

    auto color = std::make_shared<cga::node::Color>();
    color->SetColor({ 1, 0, 0 });
    eval.AddNode(color);

    eval.Connect({ extrude, 0 }, { color, 0 });

    auto rule_eval = eval.ToRule();

    SECTION("quad")
    {
        auto quad = std::make_shared<cga::node::PrimQuad>();
        quad->SetLength(1);
        quad->SetWidth(2);

        std::vector<cga::GeoPtr> geos;
        quad->Execute(std::vector<cga::GeoPtr>(), geos, cga::EvalContext());

        auto dst_geos = rule_eval->Eval(geos);

        REQUIRE(dst_geos.size() == 1);
        test::check_points_num(*dst_geos[0], 8);
        test::check_faces_num(*dst_geos[0], 6);
        test::check_aabb(*dst_geos[0], { 0, 0, 0 }, { 1, 2, 1 });
        test::check_color(*dst_geos[0], { 1, 0, 0 });
    }
}