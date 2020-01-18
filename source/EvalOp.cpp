#include "cga/EvalOp.h"
#include "cga/Operation.h"

#include <stack>
#include <queue>

#include <assert.h>

namespace cga
{

EvalOp::EvalOp(std::function<void(const std::vector<GeoPtr>&, void*)> execute_cb)
    : m_execute_cb(execute_cb)
{
}

void EvalOp::AddOp(const OpPtr& op, void* ud)
{
    auto name = op->GetName();
    while (name.empty() || m_ops_map.find(name) != m_ops_map.end())
    {
        if (op->GetName().empty()) {
            name = "op" + std::to_string(m_next_id++);
        } else {
            name = op->GetName() + std::to_string(m_next_id++);
        }
    }
    op->SetName(name);

    assert(m_ops_map.size() == m_ops_sorted.size());
    m_ops_map.insert({ name, { op, ud } });
    m_ops_sorted.insert(m_ops_sorted.begin(), { op, ud });

    m_dirty = true;
}

void EvalOp::RemoveOp(const OpPtr& op)
{
    if (m_ops_map.empty()) {
        return;
    }

    auto itr = m_ops_map.find(op->GetName());
    if (itr == m_ops_map.end()) {
        return;
    }

    SetTreeDirty(op);

    assert(m_ops_map.size() == m_ops_sorted.size());
    m_ops_map.erase(itr);
    for (auto itr = m_ops_sorted.begin(); itr != m_ops_sorted.end(); ++itr) {
        if (itr->first == op) {
            m_ops_sorted.erase(itr);
            break;
        }
    }

    m_dirty = true;
}

void EvalOp::ClearAllOps()
{
    if (m_ops_map.empty()) {
        return;
    }

    assert(m_ops_map.size() == m_ops_sorted.size());
    m_ops_map.clear();
    m_ops_sorted.clear();

    m_dirty = true;
}

void EvalOp::PropChanged(const OpPtr& op)
{
    SetTreeDirty(op);

    m_dirty = true;
}

void EvalOp::Connect(const Operation::PortAddr& from, const Operation::PortAddr& to)
{
    dag::make_connecting<OpVarType>(from, to);

    auto op = to.node.lock();
    assert(op);
    SetTreeDirty(std::static_pointer_cast<Operation>(op));

    m_dirty = true;
}

void EvalOp::Disconnect(const Operation::PortAddr& from, const Operation::PortAddr& to)
{
    dag::disconnect<OpVarType>(from, to);

    auto op = to.node.lock();
    assert(op);
    SetTreeDirty(std::static_pointer_cast<Operation>(op));

    m_dirty = true;
}

void EvalOp::RebuildConnections(const std::vector<std::pair<Operation::PortAddr, Operation::PortAddr>>& conns)
{
    // update dirty
    for (auto itr : m_ops_map) {
        if (HasOpConns(itr.second.first)) {
            SetTreeDirty(itr.second.first);
        }
    }

    // remove conns
    for (auto itr : m_ops_map)
    {
        for (auto& in : itr.second.first->GetImports()) {
            const_cast<Operation::Port&>(in).conns.clear();
        }
        for (auto& out : itr.second.first->GetExports()) {
            const_cast<Operation::Port&>(out).conns.clear();
        }
    }

    // add conns
    for (auto& conn : conns)
    {
        auto t_op = conn.second.node.lock();
        assert(t_op);
        SetTreeDirty(std::static_pointer_cast<Operation>(t_op));
        dag::make_connecting<OpVarType>(conn.first, conn.second);
    }

    m_dirty = true;
}

std::map<OpPtr, std::vector<GeoPtr>>
EvalOp::Eval(const EvalContext& ctx) const
{
    std::map<OpPtr, std::vector<GeoPtr>> op2geos;
    if (m_ops_sorted.empty()) {
        return op2geos;
    }

    TopologicalSorting();

    for (auto& pair : m_ops_sorted)
    {
        auto& op = pair.first;

        std::vector<GeoPtr> inputs, outputs;
        inputs.reserve(op->GetImports().size());
        for (auto& port : op->GetImports())
        {
            auto& conns = port.conns;
            if (conns.empty()) {
                inputs.push_back(nullptr);
                continue;
            }

            assert(conns.size() == 1);
            auto& conn = conns[0];
            auto op = conn.node.lock();
            assert(op->get_type().is_derived_from<Operation>());
            auto itr = op2geos.find(std::static_pointer_cast<Operation>(op));
            assert(itr != op2geos.end()
                && conn.idx < static_cast<int>(itr->second.size()));
            inputs.push_back(itr->second[conn.idx]);
        }

        op->Execute(inputs, outputs, ctx);
        op2geos.insert({ op, outputs });

        if (m_execute_cb && !outputs.empty()) {
            m_execute_cb(outputs, pair.second);
        }

        op->SetDirty(false);
    }

    return op2geos;
}

void EvalOp::MakeDirty(bool all_ops_dirty)
{
    m_dirty = true;

    if (all_ops_dirty)
    {
        for (auto itr : m_ops_map) {
            itr.second.first->SetDirty(true);
        }
    }
}

void EvalOp::Rename(const std::string& from, const std::string& to)
{
    auto itr_f = m_ops_map.find(from);
    if (itr_f == m_ops_map.end()) {
        assert(0);
        return;
    }

    auto op = itr_f->second.first;
    auto ud = itr_f->second.second;
    m_ops_map.erase(itr_f);

    auto itr_t = m_ops_map.find(to);
    if (itr_t == m_ops_map.end())
    {
        op->SetName(to);
    }
    else
    {
        std::string fixed;
        int idx = 0;
        do {
            fixed = to + std::to_string(idx++);
        } while (m_ops_map.find(fixed) != m_ops_map.end());

        op->SetName(fixed);
    }

    m_ops_map.insert({ op->GetName(), { op, ud } });
}

void EvalOp::TopologicalSorting() const
{
    std::vector<std::pair<OpPtr, void*>> ops;
    ops.reserve(m_ops_map.size());
    for (auto itr : m_ops_map) {
        ops.push_back(itr.second);
    }

    // prepare
    std::vector<int> in_deg(m_ops_map.size(), 0);
    std::vector<std::vector<int>> out_ops(ops.size());
    for (int i = 0, n = ops.size(); i < n; ++i)
    {
        auto& op = ops[i].first;
        for (auto& port : op->GetImports())
        {
            if (port.conns.empty()) {
                continue;
            }

            assert(port.conns.size() == 1);
            auto from = port.conns[0].node.lock();
            assert(from);
            for (int j = 0, m = ops.size(); j < m; ++j) {
                if (from == ops[j].first) {
                    in_deg[i]++;
                    out_ops[j].push_back(i);
                    break;
                }
            }
        }
    }

    // sort
    std::stack<int> st;
    m_ops_sorted.clear();
    for (int i = 0, n = in_deg.size(); i < n; ++i) {
        if (in_deg[i] == 0) {
            st.push(i);
        }
    }
    while (!st.empty())
    {
        int v = st.top();
        st.pop();
        m_ops_sorted.push_back(ops[v]);
        for (auto& i : out_ops[v]) {
            assert(in_deg[i] > 0);
            in_deg[i]--;
            if (in_deg[i] == 0) {
                st.push(i);
            }
        }
    }
}

void EvalOp::SetTreeDirty(const OpPtr& root)
{
    std::queue<const dag::Node<OpVarType>*> buf;
    buf.push(root.get());
    while (!buf.empty())
    {
        auto n = buf.front(); buf.pop();
        n->SetDirty(true);
        for (auto& port : n->GetExports()) {
            for (auto& conn : port.conns) {
                auto op = conn.node.lock();
                buf.push(op.get());
            }
        }
    }
}

bool EvalOp::HasOpConns(const OpPtr& op)
{
    for (auto& i : op->GetImports()) {
        if (!i.conns.empty()) {
            return true;
        }
    }
    for (auto& o : op->GetExports()) {
        if (!o.conns.empty()) {
            return true;
        }
    }
    return false;
}

}