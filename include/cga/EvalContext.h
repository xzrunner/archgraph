#pragma once

#include <dag/Variable.h>
#include <cgac/typedef.h>

#include <vector>

namespace cga
{

class EvalContext
{
public:
    struct Parm
    {
        Parm() {}
        Parm(const std::string& name, const dag::Variable& val)
            : name(name), val(val)
        {
        }
        Parm(const std::string& name, const cgac::ExprNodePtr& val_expr)
            : name(name), val_expr(val_expr)
        {
        }

        std::string       name;
        dag::Variable     val;
        cgac::ExprNodePtr val_expr = nullptr;
    };

public:
    void AddGlobalParm(const Parm& parm);
    void RemoveGlobalParm(const std::string& name);

    auto& GetGlobalParms() const { return m_global_parms; }
    void  SetGlobalParms(std::vector<Parm> parms) { m_global_parms = parms; }

    auto& GetLocalParms() const { return m_local_parms; }
    void  SetLocalParms(std::vector<Parm> parms) const { m_local_parms = parms; }

    void Clear();

private:
    std::vector<Parm> m_global_parms;
    mutable std::vector<Parm> m_local_parms;

}; // EvalContext

}