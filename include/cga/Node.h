#pragma once

#include "cga/typedef.h"
#include "cga/NodeVarType.h"

#include <dag/Node.h>

namespace cga
{

class Geometry;

class Node : public dag::Node<NodeVarType>
{
public:
    Node();

    virtual void Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out) = 0;

    virtual void AddInputPorts(size_t num) {}

    virtual std::shared_ptr<Geometry> GetGeo(int idx = -1) const { return m_geo; }


    RTTR_ENABLE(dag::Node<NodeVarType>)

}; // Node

}