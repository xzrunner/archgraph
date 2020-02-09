#include "ce/FuncRegister.h"
#include "ce/ShapeAttrFuncs.h"
#include "ce/BuildInFuncs.h"

namespace ce
{

CU_SINGLETON_DEFINITION(FuncRegister);

extern void regist_rttr();

FuncRegister::FuncRegister()
{
    InitFuncs();

	regist_rttr();
}

FuncPtr FuncRegister::QueryFunc(const std::string& name) const
{
    auto itr = m_funcs.find(name);
    return itr == m_funcs.end() ? nullptr : itr->second;
}

FuncPtr FuncRegister::QueryAttrFunc(const std::string& name) const
{
    auto itr = m_attr_funcs.find(name);
    return itr == m_attr_funcs.end() ? nullptr : itr->second;
}

void FuncRegister::InitFuncs()
{
    // scope attributes
    m_attr_funcs.insert({ "scope.sx", std::make_shared<func::ScopeSizeX>() });

    // built-in
    m_funcs.insert({ "print", std::make_shared<func::Print>() });
}

}