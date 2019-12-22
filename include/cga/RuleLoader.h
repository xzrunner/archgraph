#pragma once

#include "cga/Rule.h"

#include <cgac/typedef.h>

#include <string>
#include <map>
#include <vector>

namespace cgac { struct ExpressionNode; struct StatementNode; }

namespace cga
{

class EvalRule;

class RuleLoader
{
public:
    RuleLoader();

    std::shared_ptr<EvalRule>
        RunString(const std::string& str, bool debug = false);

private:
    void LoadStatement(EvalRule& eval, const cgac::StmtNodePtr& stmt);
    void LoadExpression(EvalRule& eval, const cgac::ExprNodePtr& expr);

    void FlushRule(EvalRule& eval);

private:
    RulePtr      m_curr_rule = nullptr;
    Rule::SelPtr m_curr_sel  = nullptr;

}; // RuleLoader

}