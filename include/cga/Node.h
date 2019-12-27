#pragma once

#include "cga/typedef.h"
#include "cga/NodeVarType.h"

#include <dag/Node.h>
#include <cgac/typedef.h>

namespace cga
{

class Geometry;
class EvalContext;

class Node : public dag::Node<NodeVarType>
{
public:
    Node();

    virtual void Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
        const EvalContext& ctx) = 0;

    virtual void AddInputPorts(size_t num) {}

    virtual void Setup(const std::vector<cgac::ExprNodePtr>& parms,
        const std::vector<cgac::ExprNodePtr>& selectors, const EvalContext& ctx) {}

    bool IsPortChanged() const { return m_port_changed; }
    void SetPortChanged(bool changed) { m_port_changed = changed; }

private:
    bool m_port_changed = false;

    RTTR_ENABLE(dag::Node<NodeVarType>)

}; // Node

}