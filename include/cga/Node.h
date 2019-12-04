#pragma once

#include "cga/NodeVarType.h"
#include "cga/Geometry.h"

#include <dag/Node.h>

namespace cga
{

class Node : public dag::Node<NodeVarType>
{
public:
    Node(size_t build_in_count = 0);

    virtual void Execute() = 0;

    virtual void AddInputPorts(size_t num) {}

    auto& GetGeo() const { return m_geo; }

protected:
    Geometry m_geo;

    RTTR_ENABLE(dag::Node<NodeVarType>)

}; // Node

}