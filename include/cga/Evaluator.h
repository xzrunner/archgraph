#pragma once

#include "cga/typedef.h"
#include "cga/Node.h"

#include <vector>

namespace cga
{

class Evaluator
{
public:
    Evaluator(std::function<void(const NodePtr&, void*)> execute_cb);

    // update nodes
    void AddNode(const NodePtr& node, void* ud);
    void RemoveNode(const NodePtr& node);
    void ClearAllNodes();

    // update node prop
    void PropChanged(const NodePtr& node);

    // update node conn
    void Connect(const Node::PortAddr& from, const Node::PortAddr& to);
    void Disconnect(const Node::PortAddr& from, const Node::PortAddr& to);
    void RebuildConnections(const std::vector<std::pair<Node::PortAddr, Node::PortAddr>>& conns);

    void Update();

    void MakeDirty(bool all_nodes_dirty = true);

    void Rename(const std::string& from, const std::string& to);

private:
    void UpdateNodes();

    void TopologicalSorting();

    static void SetTreeDirty(const NodePtr& root);

    static bool HasNodeConns(const NodePtr& node);

private:
    std::function<void(const NodePtr&, void*)> m_execute_cb = nullptr;

    std::map<std::string, std::pair<NodePtr, void*>> m_nodes_map;

    std::vector<std::pair<NodePtr, void*>> m_nodes_sorted;

    bool m_dirty = false;

    size_t m_next_id = 0;

}; // Evaluator

}