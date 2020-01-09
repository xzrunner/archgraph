#include "cga/EvalContext.h"

namespace cga
{

void EvalContext::AddGlobalParm(const Parm& parm)
{
    for (auto& p : m_global_parms)
    {
        if (p.name == parm.name) {
            p.val = parm.val;
            return;
        }
    }

    m_global_parms.push_back(parm);
}

void EvalContext::RemoveGlobalParm(const std::string& name)
{
    for (auto itr = m_global_parms.begin(); itr != m_global_parms.end(); )
    {
        if (itr->name == name) {
            itr = m_global_parms.erase(itr);
        } else {
            ++itr;
        }
    }
}

void EvalContext::Clear()
{
    m_global_parms.clear();
    m_local_parms.clear();
}

}