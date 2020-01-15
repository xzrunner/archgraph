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
        Parm(const std::string& name, const dag::Variable& value)
            : name(name), value(value)
        {
        }
        Parm(const std::string& name, const cgac::ExprNodePtr& expr)
            : name(name), expr(expr)
        {
        }

        std::string       name;
        dag::Variable     value;
        cgac::ExprNodePtr expr = nullptr;
    };

public:
    void AddVar(const Parm& var);
    void DeleteVar(const std::string& name);

    const Parm* QueryVar(const std::string& name) const;

    void SetVars(const std::vector<Parm>& vars) { m_vars = vars; }
    auto& GetVars() const { return m_vars; }

    void Clear() { m_vars.clear(); }

private:
    std::vector<Parm> m_vars;

}; // EvalContext

}