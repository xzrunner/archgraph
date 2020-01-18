#pragma once

#include "cga/typedef.h"

#include <cu/cu_macro.h>

#include <map>
#include <string>

namespace cga
{

class Function;

class FuncRegister
{
public:
    FuncPtr QueryFunc(const std::string& name) const;
    FuncPtr QueryAttrFunc(const std::string& name) const;

private:
    void InitFuncs();

private:
    std::map<std::string, FuncPtr> m_funcs;
    std::map<std::string, FuncPtr> m_attr_funcs;

    CU_SINGLETON_DECLARATION(FuncRegister)

}; // FuncRegister

}