#pragma once

#include "cga/NodeVarType.h"

#include <dag/Node.h>

namespace cga
{

class Geometry;

class Node : public dag::Node<NodeVarType>
{
public:
    Node();

    virtual void Execute() = 0;

    virtual void AddInputPorts(size_t num) {}

    auto& GetGeo() const { return m_geo; }

protected:
    std::shared_ptr<Geometry> m_geo = nullptr;

    RTTR_ENABLE(dag::Node<NodeVarType>)

}; // Node

}