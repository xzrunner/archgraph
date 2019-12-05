#include "cga/NodeHelper.h"
#include "cga/Node.h"

namespace cga
{

std::shared_ptr<Geometry> NodeHelper::GetInputGeo(const Node& node, size_t idx)
{
    auto& imports = node.GetImports();
    if (idx < 0 || idx >= imports.size()) {
        return nullptr;
    }

    auto& conns = imports[idx].conns;
    if (conns.empty()) {
        return nullptr;
    }

    assert(conns.size() == 1);
    auto& conn = conns.front();
    auto in_node = conn.node.lock();
    if (!in_node) {
        return nullptr;
    }
    assert(in_node->get_type().is_derived_from<Node>());
    return std::static_pointer_cast<Node>(in_node)->GetGeo();
}

}