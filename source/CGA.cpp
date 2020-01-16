#include "cga/CGA.h"

#include <cgac/Type.h>
#include <cgac/StringPool.h>

namespace cga
{

CU_SINGLETON_DEFINITION(CGA);

extern void regist_rttr();

CGA::CGA()
{
    m_str_pool = std::make_shared<cgac::StringPool>();

	regist_rttr();

    cgac::SetupTypeSystem();
}

}