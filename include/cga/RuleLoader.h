#pragma once

#include "cga/Rule.h"

#include <cgac/typedef.h>

#include <string>
#include <map>
#include <vector>

namespace cgac { struct ExpressionNode; struct StatementNode; class Parser; }

namespace cga
{

class EvalRule;

class RuleLoader
{
public:
    RuleLoader();

    bool RunString(const std::string& str, EvalRule& eval, bool debug = false);

private:
    void LoadStatement(EvalRule& eval, const cgac::StmtNodePtr& stmt);
    void LoadExpression(EvalRule& eval, const cgac::ExprNodePtr& expr);

    void FlushRule(EvalRule& eval);

private:
    RulePtr      m_curr_rule = nullptr;
    Rule::SelPtr m_curr_sel  = nullptr;

    std::vector<std::shared_ptr<cgac::Parser>> m_parsers;

}; // RuleLoader

}