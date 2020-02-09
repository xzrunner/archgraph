#include "ce/EvalContext.h"

namespace ce
{

void EvalContext::AddVar(const Parm& var)
{
    for (auto& p : m_vars)
    {
        if (p.name == var.name) {
            p.expr = var.expr;
            p.value = var.value;
            return;
        }
    }

    m_vars.push_back(var);
}

void EvalContext::DeleteVar(const std::string& name)
{
    for (auto itr = m_vars.begin(); itr != m_vars.end(); )
    {
        if (itr->name == name) {
            itr = m_vars.erase(itr);
        } else {
            ++itr;
        }
    }
}

const EvalContext::Parm*
EvalContext::QueryVar(const std::string& name) const
{
    for (auto& prop : m_vars) {
        if (prop.name == name) {
            return &prop;
        }
    }
    return nullptr;
}

//////////////////////////////////////////////////////////////////////////
// class EvalContext::Parm
//////////////////////////////////////////////////////////////////////////

EvalContext::Parm::
Parm(const std::string& name, const dag::Variable& value)
    : name(name), value(value)
{
}

EvalContext::Parm::
Parm(const std::string& name, const cgac::ExprNodePtr& expr)
    : name(name), expr(expr)
{
}

}