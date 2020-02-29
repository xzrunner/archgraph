#include "archgraph/ArchGraph.h"

#include <cga/Type.h>
#include <cga/StringPool.h>

namespace archgraph
{

CU_SINGLETON_DEFINITION(ArchGraph);

extern void regist_rttr();

ArchGraph::ArchGraph()
{
    m_str_pool = std::make_shared<cga::StringPool>();

	regist_rttr();

    cga::SetupTypeSystem();
}

}