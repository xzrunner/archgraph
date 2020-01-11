#include "cga/CGA.h"
#include "cga/ShapeAttrFuncs.h"
#include "cga/BuildInFuncs.h"

namespace cga
{

CU_SINGLETON_DEFINITION(CGA);

extern void regist_rttr();

CGA::CGA()
{
    InitFuncs();

	regist_rttr();
}

FuncPtr CGA::QueryFunc(const std::string& name) const
{
    auto itr = m_funcs.find(name);
    return itr == m_funcs.end() ? nullptr : itr->second;
}

void CGA::InitFuncs()
{
    // scope attributes
    m_funcs.insert({ "scope.sx", std::make_shared<func::ScopeSizeX>() });

    // built-in
    m_funcs.insert({ "print", std::make_shared<func::Print>() });
}

}