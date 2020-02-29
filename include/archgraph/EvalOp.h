#pragma once

#include "archgraph/typedef.h"
#include "archgraph/Operation.h"
#include "archgraph/EvalContext.h"

namespace archgraph
{

class EvalRule;

class EvalOp
{
public:
    EvalOp(std::function<void(const std::vector<GeoPtr>&, void*)> execute_cb = nullptr);

    // update operations
    void AddOp(const OpPtr& op, void* ud = nullptr);
    void RemoveOp(const OpPtr& op);
    void ClearAllOps();

    // update op prop
    void PropChanged(const OpPtr& op);

    // update op conn
    void Connect(const Operation::PortAddr& from, const Operation::PortAddr& to);
    void Disconnect(const Operation::PortAddr& from, const Operation::PortAddr& to);
    void RebuildConnections(const std::vector<std::pair<Operation::PortAddr, Operation::PortAddr>>& conns);

    std::map<OpPtr, std::vector<GeoPtr>> Eval(const EvalContext& ctx) const;
    std::shared_ptr<EvalRule> ToRule(const EvalContext& ctx) const;

    void MakeDirty(bool all_ops_dirty = true);

    void Rename(const std::string& from, const std::string& to);

private:
    static void SetTreeDirty(const OpPtr& root);

    static bool HasOpConns(const OpPtr& op);

private:
    std::function<void(const std::vector<GeoPtr>&, void*)> m_execute_cb = nullptr;

    std::map<std::string, std::pair<OpPtr, void*>> m_ops_map;

    bool m_dirty = false;

    size_t m_next_id = 0;

}; // EvalOp

}