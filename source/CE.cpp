#include "ce/CE.h"

#include <cgac/Type.h>
#include <cgac/StringPool.h>

namespace ce
{

CU_SINGLETON_DEFINITION(CE);

extern void regist_rttr();

CE::CE()
{
    m_str_pool = std::make_shared<cgac::StringPool>();

	regist_rttr();

    cgac::SetupTypeSystem();
}

}