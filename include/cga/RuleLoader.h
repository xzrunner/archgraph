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
    struct Context
    {
        std::vector<Rule::SelPtr> selectors;
        std::vector<Rule::OpPtr>  operators;

        RulePtr rule = nullptr;

        void Flush(EvalRule& eval, bool dup = false);
    };

private:
    void LoadStatement(EvalRule& eval, const cgac::StmtNodePtr& stmt, Context& ctx);
    void LoadExpression(EvalRule& eval, const cgac::ExprNodePtr& expr, Context& ctx);

private:
    std::vector<std::shared_ptr<cgac::Parser>> m_parsers;

}; // RuleLoader

}