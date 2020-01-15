#pragma once

#include <memory>

namespace cga
{

class Operation;
using OpPtr = std::shared_ptr<Operation>;

class Geometry;
using GeoPtr = std::shared_ptr<Geometry>;

class Rule;
using RulePtr = std::shared_ptr<Rule>;

class Function;
using FuncPtr = std::shared_ptr<Function>;

class Variant;
using VarPtr = std::shared_ptr<Variant>;

}