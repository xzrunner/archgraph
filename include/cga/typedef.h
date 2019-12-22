#pragma once

#include <memory>

namespace cga
{

class Node;
using NodePtr = std::shared_ptr<Node>;

class Geometry;
using GeoPtr = std::shared_ptr<Geometry>;

class Rule;
using RulePtr = std::shared_ptr<Rule>;

}