#pragma once

#include <dag/Variable.h>
#include <cga/typedef.h>

#include <vector>

namespace archgraph
{

class EvalContext
{
public:
    struct Parm
    {
        Parm() {}
        Parm(const std::string& name, const dag::Variable& value);
        Parm(const std::string& name, const cga::ExprNodePtr& expr);

        std::string       name;
        dag::Variable     value;
        cga::ExprNodePtr expr = nullptr;
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