#pragma once

#include "archgraph/typedef.h"
#include "archgraph/OpVarType.h"
#include "archgraph/Rule.h"

#include <dag/Node.h>
#include <cga/typedef.h>

namespace archgraph
{

class Geometry;
class EvalContext;

class Operation : public dag::Node<OpVarType>
{
public:
    Operation();

    virtual void Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
        const EvalContext& ctx) = 0;

    virtual void AddInputPorts(size_t num) {}

    virtual void Setup(const std::vector<cga::ExprNodePtr>& parms,
        const Rule::CompoundSel& selectors, const EvalContext& ctx) {}

    bool IsPortChanged() const { return m_port_changed; }
    void SetPortChanged(bool changed) { m_port_changed = changed; }

    auto& GetExprsMap() const { return m_exprs_map; }

protected:
    std::map<std::string, std::string> m_exprs_map;

private:
    bool m_port_changed = false;

    RTTR_ENABLE(dag::Node<OpVarType>)

}; // Operation

}