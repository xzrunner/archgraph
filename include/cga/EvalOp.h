#pragma once

#include "cga/typedef.h"
#include "cga/Operation.h"
#include "cga/EvalContext.h"

#include <vector>

namespace cga
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

    std::map<OpPtr, std::vector<GeoPtr>> Eval() const;
    std::shared_ptr<EvalRule> ToRule() const;

    void MakeDirty(bool all_ops_dirty = true);

    void Rename(const std::string& from, const std::string& to);

private:
    void TopologicalSorting() const;

    static void SetTreeDirty(const OpPtr& root);

    static bool HasOpConns(const OpPtr& op);

private:
    std::function<void(const std::vector<GeoPtr>&, void*)> m_execute_cb = nullptr;

    std::map<std::string, std::pair<OpPtr, void*>> m_ops_map;

    mutable std::vector<std::pair<OpPtr, void*>> m_ops_sorted;

    bool m_dirty = false;

    size_t m_next_id = 0;

    EvalContext m_ctx;

}; // EvalOp

}