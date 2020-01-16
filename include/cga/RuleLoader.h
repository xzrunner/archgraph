#pragma once

#include "cga/Rule.h"

#include <cgac/typedef.h>

#include <string>
#include <map>
#include <vector>

namespace cgac
{
    struct ExpressionNode;
    struct StatementNode;
    class Parser;
    class StringPool;
}

namespace cga
{

class EvalRule;

class RuleLoader
{
public:
    RuleLoader(const std::shared_ptr<cgac::StringPool>& str_pool = nullptr);

    bool RunString(const std::string& str, EvalRule& eval, bool debug = false);

private:
    struct Context
    {
        std::vector<Rule::SelPtr> selectors;
        std::vector<Rule::OpPtr>  operators;

        void FlushOperators(const RulePtr& rule);
        void FlushSelectors(bool repeat);
    };

private:
    cgac::StmtNodePtr LoadStatement(EvalRule& eval, const cgac::StmtNodePtr& stmt, Context& ctx);
    void LoadExpression(EvalRule& eval, const cgac::ExprNodePtr& expr, Context& ctx);

private:
    std::shared_ptr<cgac::StringPool> m_str_pool = nullptr;

    std::vector<std::shared_ptr<cgac::Parser>> m_parsers;

}; // RuleLoader

}