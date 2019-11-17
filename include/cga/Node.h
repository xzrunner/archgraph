#pragma once

#include "cga/Geometry.h"

#include <rttr/registration>

namespace cga
{

class Node
{
public:
    Node(size_t build_in_count = 0);

    virtual void Execute() = 0;

    virtual void AddInputPorts(size_t num) {}

    struct Port;
    void SetImports(const std::vector<Port>& imports) { m_imports = imports; }
    void SetExports(const std::vector<Port>& exports) { m_exports = exports; }

    auto& GetImports() const { return m_imports; }
    auto& GetExports() const { return m_exports; }

    bool IsDirty() const { return m_dirty; }
    void SetDirty(bool dirty) const { m_dirty = dirty; }

    //void SetParent(const std::shared_ptr<Node>& node);
    //auto GetParent() const { return m_parent.lock(); }

    void SetName(const std::string& name) { m_name = name; }
    auto& GetName() const { return m_name; }

    auto& GetGeo() const { return m_geo; }

    void ClearConnections();

public:
    struct PortAddr
    {
        PortAddr() {}
        PortAddr(const std::shared_ptr<Node>& node, int idx)
            : node(node), idx(idx) {}
        PortAddr(const PortAddr& addr)
            : node(addr.node), idx(addr.idx) {}

        std::weak_ptr<Node> node;
        int idx = -1;
    };

    struct Port
    {
        Port() {}
        Port(const std::string& name) : name(name) {}

        std::string name;
        std::vector<PortAddr> conns;
    };

protected:
    std::vector<Port> m_imports, m_exports;

    Geometry m_geo;

private:
    std::string m_name;

//    std::weak_ptr<Node> m_parent;

    mutable bool m_dirty = true;

    RTTR_ENABLE()

}; // Node

void make_connecting(const Node::PortAddr& from, const Node::PortAddr& to);
void disconnect(const Node::PortAddr& from, const Node::PortAddr& to);

}