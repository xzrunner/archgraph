#pragma once

#include <cu/cu_macro.h>

#include <memory>

namespace cgac { class StringPool; }

namespace cga
{

class CGA
{
public:
    auto GetStringPool() const { return m_str_pool; }

private:
    std::shared_ptr<cgac::StringPool> m_str_pool = nullptr;

    CU_SINGLETON_DECLARATION(CGA)
}; // CGA

}