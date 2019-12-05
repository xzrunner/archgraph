#pragma once

#include <memory>

namespace cga
{

class Geometry;
class Node;

class NodeHelper
{
public:
    static std::shared_ptr<Geometry>
        GetInputGeo(const Node& node, size_t idx);

}; // NodeHelper

}