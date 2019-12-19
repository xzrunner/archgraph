#include "utility.h"

#include <cga/NodeHelper.h>
#include <cga/node/Color.h>

#include <cga/Evaluator.h>
#include <cga/RuleLoader.h>

#include <catch/catch.hpp>

TEST_CASE("load rule")
{
    test::init();

    cga::Evaluator eval;

    cga::RuleLoader loader;
    loader.RunString(R"(
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
)", eval);
}