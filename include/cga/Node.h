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

    virtual std::shared_ptr<Geometry> GetGeo(int idx = -1) const { return m_geo; }

protected:
    std::shared_ptr<Geometry> m_geo = nullptr;

    RTTR_ENABLE(dag::Node<NodeVarType>)

}; // Node

}