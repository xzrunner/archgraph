#include "cga/CGA.h"

namespace cga
{

CU_SINGLETON_DEFINITION(CGA);

extern void regist_rttr();

CGA::CGA()
{
	regist_rttr();
}

}