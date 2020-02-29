#pragma once

#include "archgraph/Rule.h"

#include <cga/typedef.h>

#include <string>
#include <map>
#include <vector>

namespace cga
{
    struct ExpressionNode;
    struct StatementNode;
    class Parser;
    class StringPool;
}

namespace archgraph
{

class EvalRule;

class RuleLoader
{
public:
    RuleLoader(const std::shared_ptr<cga::StringPool>& str_pool = nullptr);

    bool RunString(const EvalContext& ctx, const std::string& str,
        EvalRule& eval, bool debug = false);

private:
    struct Context
    {
        std::vector<Rule::SelPtr> selectors;
        std::vector<Rule::OpPtr>  operators;

        void FlushOperators(const RulePtr& rule);
        void FlushSelectors(bool repeat);
    };

private:
    cga::StmtNodePtr LoadStatement(EvalRule& eval, const cga::StmtNodePtr& stmt,
        Context& ctx, const EvalContext& eval_ctx);
    void LoadExpression(EvalRule& eval, const cga::ExprNodePtr& expr,
        Context& ctx, const EvalContext& eval_ctx);

private:
    std::shared_ptr<cga::StringPool> m_str_pool = nullptr;

    std::vector<std::shared_ptr<cga::Parser>> m_parsers;

}; // RuleLoader

}