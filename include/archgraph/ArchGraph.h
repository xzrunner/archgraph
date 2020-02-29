#pragma once

#include <cu/cu_macro.h>

#include <memory>

namespace cga { class StringPool; }

namespace archgraph
{

class ArchGraph
{
public:
    auto GetStringPool() const { return m_str_pool; }

private:
    std::shared_ptr<cga::StringPool> m_str_pool = nullptr;

    CU_SINGLETON_DECLARATION(ArchGraph)
}; // ArchGraph

}