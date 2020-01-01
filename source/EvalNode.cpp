#include "cga/EvalNode.h"
#include "cga/Node.h"

#include <stack>
#include <queue>

#include <assert.h>

namespace cga
{

EvalNode::EvalNode(std::function<void(const GeoPtr&, void*)> execute_cb)
    : m_execute_cb(execute_cb)
{
}

void EvalNode::AddNode(const NodePtr& node, void* ud)
{
    auto name = node->GetName();
    while (name.empty() || m_nodes_map.find(name) != m_nodes_map.end())
    {
        if (node->GetName().empty()) {
            name = "node" + std::to_string(m_next_id++);
        } else {
            name = node->GetName() + std::to_string(m_next_id++);
        }
    }
    node->SetName(name);

    assert(m_nodes_map.size() == m_nodes_sorted.size());
    m_nodes_map.insert({ name, { node, ud } });
    m_nodes_sorted.insert(m_nodes_sorted.begin(), { node, ud });

    m_dirty = true;
}

void EvalNode::RemoveNode(const NodePtr& node)
{
    if (m_nodes_map.empty()) {
        return;
    }

    auto itr = m_nodes_map.find(node->GetName());
    if (itr == m_nodes_map.end()) {
        return;
    }

    SetTreeDirty(node);

    assert(m_nodes_map.size() == m_nodes_sorted.size());
    m_nodes_map.erase(itr);
    for (auto itr = m_nodes_sorted.begin(); itr != m_nodes_sorted.end(); ++itr) {
        if (itr->first == node) {
            m_nodes_sorted.erase(itr);
            break;
        }
    }

    m_dirty = true;
}

void EvalNode::ClearAllNodes()
{
    if (m_nodes_map.empty()) {
        return;
    }

    assert(m_nodes_map.size() == m_nodes_sorted.size());
    m_nodes_map.clear();
    m_nodes_sorted.clear();

    m_dirty = true;
}

void EvalNode::PropChanged(const NodePtr& node)
{
    SetTreeDirty(node);

    m_dirty = true;
}

void EvalNode::Connect(const Node::PortAddr& from, const Node::PortAddr& to)
{
    dag::make_connecting<NodeVarType>(from, to);

    auto node = to.node.lock();
    assert(node);
    SetTreeDirty(std::static_pointer_cast<Node>(node));

    m_dirty = true;
}

void EvalNode::Disconnect(const Node::PortAddr& from, const Node::PortAddr& to)
{
    dag::disconnect<NodeVarType>(from, to);

    auto node = to.node.lock();
    assert(node);
    SetTreeDirty(std::static_pointer_cast<Node>(node));

    m_dirty = true;
}

void EvalNode::RebuildConnections(const std::vector<std::pair<Node::PortAddr, Node::PortAddr>>& conns)
{
    // update dirty
    for (auto itr : m_nodes_map) {
        if (HasNodeConns(itr.second.first)) {
            SetTreeDirty(itr.second.first);
        }
    }

    // remove conns
    for (auto itr : m_nodes_map)
    {
        for (auto& in : itr.second.first->GetImports()) {
            const_cast<Node::Port&>(in).conns.clear();
        }
        for (auto& out : itr.second.first->GetExports()) {
            const_cast<Node::Port&>(out).conns.clear();
        }
    }

    // add conns
    for (auto& conn : conns)
    {
        auto t_node = conn.second.node.lock();
        assert(t_node);
        SetTreeDirty(std::static_pointer_cast<Node>(t_node));
        dag::make_connecting<NodeVarType>(conn.first, conn.second);
    }

    m_dirty = true;
}

std::map<NodePtr, std::vector<GeoPtr>>
EvalNode::Eval() const
{
    std::map<NodePtr, std::vector<GeoPtr>> node2geos;
    if (m_nodes_sorted.empty()) {
        return node2geos;
    }

    TopologicalSorting();

    for (auto& pair : m_nodes_sorted)
    {
        auto& node = pair.first;

        std::vector<GeoPtr> inputs, outputs;
        inputs.reserve(node->GetImports().size());
        for (auto& port : node->GetImports())
        {
            auto& conns = port.conns;
            if (conns.empty()) {
                inputs.push_back(nullptr);
                continue;
            }

            assert(conns.size() == 1);
            auto& conn = conns[0];
            auto node = conn.node.lock();
            assert(node->get_type().is_derived_from<Node>());
            auto itr = node2geos.find(std::static_pointer_cast<Node>(node));
            assert(itr != node2geos.end()
                && conn.idx < static_cast<int>(itr->second.size()));
            inputs.push_back(itr->second[conn.idx]);
        }

        node->Execute(inputs, outputs, m_ctx);
        node2geos.insert({ node, outputs });

        if (m_execute_cb && !outputs.empty()) {
            m_execute_cb(outputs[0], pair.second);
        }

        node->SetDirty(false);
    }

    return node2geos;
}

void EvalNode::MakeDirty(bool all_nodes_dirty)
{
    m_dirty = true;

    if (all_nodes_dirty)
    {
        for (auto itr : m_nodes_map) {
            itr.second.first->SetDirty(true);
        }
    }
}

void EvalNode::Rename(const std::string& from, const std::string& to)
{
    auto itr_f = m_nodes_map.find(from);
    if (itr_f == m_nodes_map.end()) {
        assert(0);
        return;
    }

    auto node = itr_f->second.first;
    auto ud = itr_f->second.second;
    m_nodes_map.erase(itr_f);

    auto itr_t = m_nodes_map.find(to);
    if (itr_t == m_nodes_map.end())
    {
        node->SetName(to);
    }
    else
    {
        std::string fixed;
        int idx = 0;
        do {
            fixed = to + std::to_string(idx++);
        } while (m_nodes_map.find(fixed) != m_nodes_map.end());

        node->SetName(fixed);
    }

    m_nodes_map.insert({ node->GetName(), { node, ud } });
}

void EvalNode::TopologicalSorting() const
{
    std::vector<std::pair<NodePtr, void*>> nodes;
    nodes.reserve(m_nodes_map.size());
    for (auto itr : m_nodes_map) {
        nodes.push_back(itr.second);
    }

    // prepare
    std::vector<int> in_deg(m_nodes_map.size(), 0);
    std::vector<std::vector<int>> out_nodes(nodes.size());
    for (int i = 0, n = nodes.size(); i < n; ++i)
    {
        auto& node = nodes[i].first;
        for (auto& port : node->GetImports())
        {
            if (port.conns.empty()) {
                continue;
            }

            assert(port.conns.size() == 1);
            auto from = port.conns[0].node.lock();
            assert(from);
            for (int j = 0, m = nodes.size(); j < m; ++j) {
                if (from == nodes[j].first) {
                    in_deg[i]++;
                    out_nodes[j].push_back(i);
                    break;
                }
            }
        }
    }

    // sort
    std::stack<int> st;
    m_nodes_sorted.clear();
    for (int i = 0, n = in_deg.size(); i < n; ++i) {
        if (in_deg[i] == 0) {
            st.push(i);
        }
    }
    while (!st.empty())
    {
        int v = st.top();
        st.pop();
        m_nodes_sorted.push_back(nodes[v]);
        for (auto& i : out_nodes[v]) {
            assert(in_deg[i] > 0);
            in_deg[i]--;
            if (in_deg[i] == 0) {
                st.push(i);
            }
        }
    }
}

void EvalNode::SetTreeDirty(const NodePtr& root)
{
    std::queue<const dag::Node<NodeVarType>*> buf;
    buf.push(root.get());
    while (!buf.empty())
    {
        auto n = buf.front(); buf.pop();
        n->SetDirty(true);
        for (auto& port : n->GetExports()) {
            for (auto& conn : port.conns) {
                auto node = conn.node.lock();
                buf.push(node.get());
            }
        }
    }
}

bool EvalNode::HasNodeConns(const NodePtr& node)
{
    for (auto& i : node->GetImports()) {
        if (!i.conns.empty()) {
            return true;
        }
    }
    for (auto& o : node->GetExports()) {
        if (!o.conns.empty()) {
            return true;
        }
    }
    return false;
}

}