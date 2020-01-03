#pragma once

#include "cga/typedef.h"
#include "cga/Node.h"
#include "cga/EvalContext.h"

#include <vector>

namespace cga
{

class EvalRule;

class EvalNode
{
public:
    EvalNode(std::function<void(const GeoPtr&, void*)> execute_cb = nullptr);

    // update nodes
    void AddNode(const NodePtr& node, void* ud = nullptr);
    void RemoveNode(const NodePtr& node);
    void ClearAllNodes();

    // update node prop
    void PropChanged(const NodePtr& node);

    // update node conn
    void Connect(const Node::PortAddr& from, const Node::PortAddr& to);
    void Disconnect(const Node::PortAddr& from, const Node::PortAddr& to);
    void RebuildConnections(const std::vector<std::pair<Node::PortAddr, Node::PortAddr>>& conns);

    std::map<NodePtr, std::vector<GeoPtr>> Eval() const;
    std::shared_ptr<EvalRule> ToRule() const;

    void MakeDirty(bool all_nodes_dirty = true);

    void Rename(const std::string& from, const std::string& to);

private:
    void TopologicalSorting() const;

    static void SetTreeDirty(const NodePtr& root);

    static bool HasNodeConns(const NodePtr& node);

private:
    std::function<void(const GeoPtr&, void*)> m_execute_cb = nullptr;

    std::map<std::string, std::pair<NodePtr, void*>> m_nodes_map;

    mutable std::vector<std::pair<NodePtr, void*>> m_nodes_sorted;

    bool m_dirty = false;

    size_t m_next_id = 0;

    EvalContext m_ctx;

}; // EvalNode

}