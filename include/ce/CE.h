#pragma once

#include <cu/cu_macro.h>

#include <memory>

namespace cgac { class StringPool; }

namespace ce
{

class CE
{
public:
    auto GetStringPool() const { return m_str_pool; }

private:
    std::shared_ptr<cgac::StringPool> m_str_pool = nullptr;

    CU_SINGLETON_DECLARATION(CE)
}; // CE

}