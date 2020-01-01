#include "cga/RuleLoader.h"
#include "cga/EvalRule.h"

#include <cgac/Parser.h>
#include <cgac/Statement.h>
#include <cgac/DumpAST.h>
#include <cgac/Statement.h>

#include <iostream>

namespace cga
{

RuleLoader::RuleLoader()
{
    cgac::SetupTypeSystem();
}

bool RuleLoader::RunString(const std::string& str, EvalRule& eval, bool debug)
{
    if (str.empty()) {
        return false;
    }

    const std::string s = "{" + str + "}";
    auto parser = std::make_shared<cgac::Parser>(s.c_str());
    m_parsers.push_back(parser);
    auto ast = cgac::StatementParser::ParseStatement(*parser);

    if (debug) {
        cgac::DumpStatement(std::cout, ast, 0);
    }

    Context ctx;
    LoadStatement(eval, ast, ctx);
    ctx.Flush(eval);

    eval.OnLoadFinished();

    return true;
}

void RuleLoader::LoadStatement(EvalRule& eval, const cgac::StmtNodePtr& stmt, Context& ctx)
{
    switch (stmt->kind)
    {
    case cgac::NK_ExpressionStatement:
        LoadExpression(eval, std::static_pointer_cast<cgac::ExprStmtNode>(stmt)->expr, ctx);
        break;

    case cgac::NK_CompoundStatement:
    {
        auto p = std::static_pointer_cast<cgac::CompoundStmtNode>(stmt)->stmts;
        while (p != NULL) {
            LoadStatement(eval, std::static_pointer_cast<cgac::StatementNode>(p), ctx);
            p = p->next;
        }
        if (std::static_pointer_cast<cgac::CompoundStmtNode>(stmt)->duplicate)
        {
            ctx.Flush(eval, true);
        }
    }
        break;
    }
}

void RuleLoader::LoadExpression(EvalRule& eval, const cgac::ExprNodePtr& expr, Context& ctx)
{
    switch (expr->op)
    {
    case cgac::OP_ASSIGN:
    {
        switch (expr->kids[0]->op)
        {
        case cgac::OP_ID:
            eval.AddSymbol((char*)(expr->kids[0]->val.p), expr->kids[1]);
            break;

        default:
            assert(0);
        }
    }
        break;

    case cgac::OP_SEPARATOR:
    {
        LoadExpression(eval, expr->kids[0], ctx);
        LoadExpression(eval, expr->kids[1], ctx);
    }
        break;

    case cgac::OP_COLON:
    {
        auto sel = std::make_shared<Rule::SingleSel>();
        sel->head = expr->kids[0];

        Context sub_ctx;
        LoadExpression(eval, expr->kids[1], sub_ctx);

        sel->ops = sub_ctx.operators;
        assert(!sub_ctx.rule && sub_ctx.selectors.empty());

        ctx.selectors.push_back(sel);
    }
        break;

    case cgac::OP_RULE:
    {
        std::string rule_name;
        std::vector<std::string> rule_params;
        switch (expr->kids[0]->op)
        {
        case cgac::OP_CALL:
        {
            assert(expr->kids[0]->kids[0]->op == cgac::OP_ID);
            rule_name = (char*)(expr->kids[0]->kids[0]->val.p);

            auto p = expr->kids[0]->kids[1];
            while (p) {
                assert(p->op == cgac::OP_ID);
                rule_params.push_back(static_cast<char*>(p->val.p));
                p = std::static_pointer_cast<cgac::ExpressionNode>(p->next);
            }
        }
            break;

        case cgac::OP_ID:
            rule_name = static_cast<char*>(expr->kids[0]->val.p);
            break;

        default:
            assert(0);
        }

        ctx.Flush(eval);

        ctx.rule = std::make_shared<Rule>(rule_name, rule_params);
        LoadExpression(eval, expr->kids[1], ctx);
    }
        break;

    case cgac::OP_CALL:
    {
        auto op = std::make_shared<Rule::Operator>();

        assert(expr->kids[0]->op == cgac::OP_ID);
        op->name = (char*)(expr->kids[0]->val.p);

        auto p = expr->kids[1];
        while (p) {
            op->params.push_back(p);
            p = std::static_pointer_cast<cgac::ExpressionNode>(p->next);
        }

        ctx.operators.push_back(op);
    }
        break;

    case cgac::OP_DUPLICATE:
    {
        Context sub_ctx;
        LoadExpression(eval, expr->kids[0], sub_ctx);
        if (!sub_ctx.selectors.empty())
        {
            assert(sub_ctx.operators.empty() && !sub_ctx.rule);

            Rule::SelPtr new_sel = nullptr;
            if (sub_ctx.selectors.size() == 1)
            {
                new_sel = sub_ctx.selectors[0];
            }
            else
            {
                auto comp_sel = std::make_shared<Rule::CompoundSel>();
                comp_sel->sels = sub_ctx.selectors;
                new_sel = comp_sel;
            }
            assert(new_sel);
            new_sel->duplicate = true;
            ctx.selectors.push_back(new_sel);
        }
    }
        break;

    case cgac::OP_ID:
    {
        auto op = std::make_shared<Rule::Operator>();
        op->name = (char*)(expr->val.p);
        ctx.operators.push_back(op);
    }
        break;

    case cgac::OP_ATTR:
        break;

    default:
        assert(0);
    }
}

//////////////////////////////////////////////////////////////////////////
// class RuleLoader::Context
//////////////////////////////////////////////////////////////////////////

void RuleLoader::Context::Flush(EvalRule& eval, bool dup)
{
    if (rule)
    {
        if (!operators.empty() && !selectors.empty()) 
        {
            auto& op = operators.back();
            assert(op->selectors.sels.empty());
            op->selectors.duplicate = dup;
            std::copy(selectors.begin(), selectors.end(), std::back_inserter(op->selectors.sels));
            selectors.clear();
        }

        if (!operators.empty()) {
            for (auto& op : operators) {
                rule->AddOperator(op);
            }
            operators.clear();
        }

        eval.AddRule(rule);

        assert(selectors.empty() && operators.empty());
        rule.reset();
    }
    else
    {
        assert(operators.empty() && selectors.empty());
    }
}

}