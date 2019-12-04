#pragma once

#include "cga/NodeVarType.h"
#include "cga/Geometry.h"

#include <hdiop/Node.h>

namespace cga
{

class Node : public hdiop::Node<NodeVarType>
{
public:
    Node(size_t build_in_count = 0);

    virtual void Execute() = 0;

    virtual void AddInputPorts(size_t num) {}

    auto& GetGeo() const { return m_geo; }

protected:
    Geometry m_geo;

    RTTR_ENABLE(hdiop::Node<NodeVarType>)

}; // Node

}