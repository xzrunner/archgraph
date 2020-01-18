#include "cga/RuleLoader.h"
#include "cga/EvalRule.h"

#include <cgac/Parser.h>
#include <cgac/Statement.h>
#include <cgac/DumpAST.h>
#include <cgac/Statement.h>

#include <iostream>

namespace cga
{

RuleLoader::RuleLoader(const std::shared_ptr<cgac::StringPool>& str_pool)
    : m_str_pool(str_pool)
{
}

bool RuleLoader::RunString(const EvalContext& eval_ctx, const std::string& str,
                           EvalRule& eval, bool debug)
{
    if (str.empty()) {
        return false;
    }

    auto parser = std::make_shared<cgac::Parser>(str.c_str(), m_str_pool);
    m_parsers.push_back(parser);
    auto ast = cgac::StatementParser::ParseStatement(*parser);

    if (debug) {
        cgac::DumpStatement(std::cout, ast, 0);
    }

    Context ctx;
    LoadStatement(eval, ast, ctx, eval_ctx);

    eval.OnLoadFinished(eval_ctx);

    return true;
}

cgac::StmtNodePtr
RuleLoader::LoadStatement(EvalRule& eval, const cgac::StmtNodePtr& stmt,
                          Context& ctx, const EvalContext& eval_ctx)
{
    auto ret = stmt->next;

    switch (stmt->kind)
    {
    case cgac::NK_ExpressionStatement:
        LoadExpression(eval, std::static_pointer_cast<cgac::ExprStmtNode>(stmt)->expr, ctx, eval_ctx);
        break;

    case cgac::NK_CompoundStatement:
    {
        auto p = std::static_pointer_cast<cgac::CompoundStmtNode>(stmt)->stmts;
        while (p != NULL) {
            p = LoadStatement(eval, std::static_pointer_cast<cgac::StatementNode>(p), ctx, eval_ctx);
        }
    }
        break;

    case cgac::NK_RuleStatement:
    {
        auto rule_stmt = std::static_pointer_cast<cgac::RuleStmtNode>(stmt);
        std::string rule_name = rule_stmt->name;

        std::vector<std::string> rule_params;
        auto parm = rule_stmt->params;
        while (parm)
        {
            assert(parm->op == cgac::OP_ID);
            rule_params.push_back(static_cast<const char*>(parm->val.p));
            parm = std::static_pointer_cast<cgac::ExpressionNode>(parm->next);
        }

        auto p = rule_stmt->stmts;
        while (p != NULL) {
            p = LoadStatement(eval, std::static_pointer_cast<cgac::StatementNode>(p), ctx, eval_ctx);
        }

        auto rule = std::make_shared<Rule>(rule_name, rule_params);
        ctx.FlushOperators(rule);
        eval.AddRule(rule);
    }
        break;

    case cgac::NK_SelectorStatement:
    {
        auto sel_stmt = std::static_pointer_cast<cgac::SelectorStmtNode>(stmt);
        LoadExpression(eval, sel_stmt->expr, ctx, eval_ctx);
        ctx.FlushSelectors(sel_stmt->repeat);
    }
        break;

    case cgac::NK_CaseStatement:
    {
        std::vector<RulePtr> rules;

        std::vector<cgac::ExprNodePtr> case_exprs;

        cgac::NodePtr item = stmt;
        while (item != NULL)
        {
            switch (item->kind)
            {
            case cgac::NK_CaseStatement:
            case cgac::NK_ElseStatement:
            {
                Context cctx;
                cgac::NodePtr p = nullptr;
                if (item->kind == cgac::NK_CaseStatement)
                {
                    auto case_stmt = std::static_pointer_cast<cgac::CaseStmtNode>(item);
                    p = case_stmt->stmts;
                    case_exprs.push_back(case_stmt->expr);
                }
                else
                {
                    p = std::static_pointer_cast<cgac::ElseStmtNode>(item)->stmts;
                }
                while (p != NULL) {
                    p = LoadStatement(eval, std::static_pointer_cast<cgac::StatementNode>(p), cctx, eval_ctx);
                }

                auto rule = std::make_shared<Rule>("");
                cctx.FlushOperators(rule);
                rules.push_back(rule);
            }
                break;
            default:
                assert(0);
            }

            if (item->kind == cgac::NK_ElseStatement) {
                break;
            }

            item = item->next;
        }

        auto op = std::make_shared<Rule::Operator>();
        op->name = "switch";
        op->type = Rule::OpType::Operation;
        op->selectors.sels.resize(rules.size());
        assert(case_exprs.size() + 1 == rules.size());
        for (size_t i = 0, n = rules.size(); i < n; ++i)
        {
            auto sel = std::make_shared<Rule::SingleSel>();
            sel->ops = rules[i]->GetAllOps();
            if (i != n - 1) {
                sel->head = case_exprs[i];
            }
            op->selectors.sels[i] = sel;
        }
        ctx.operators.push_back(op);

        ret = item->next;
    }
        break;

    default:
        assert(0);
    }

    return std::static_pointer_cast<cgac::StatementNode>(ret);
}

void RuleLoader::LoadExpression(EvalRule& eval, const cgac::ExprNodePtr& expr,
                                Context& ctx, const EvalContext& eval_ctx)
{
    switch (expr->op)
    {
    case cgac::OP_ASSIGN:
    {
        switch (expr->kids[0]->op)
        {
        case cgac::OP_ID:
            const_cast<EvalContext&>(eval_ctx).AddVar({ (char*)(expr->kids[0]->val.p), expr->kids[1] });
            break;

        default:
            assert(0);
        }
    }
        break;

    case cgac::OP_SEPARATOR:
    {
        LoadExpression(eval, expr->kids[0], ctx, eval_ctx);
        LoadExpression(eval, expr->kids[1], ctx, eval_ctx);
    }
        break;

    case cgac::OP_COLON:
    {
        auto sel = std::make_shared<Rule::SingleSel>();
        sel->head = expr->kids[0];

        Context sub_ctx;
        LoadExpression(eval, expr->kids[1], sub_ctx, eval_ctx);

        sel->ops = sub_ctx.operators;
        assert(sub_ctx.selectors.empty());

        ctx.selectors.push_back(sel);
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

    case cgac::OP_REPEAT:
    {
        Context sub_ctx;
        LoadExpression(eval, expr->kids[0], sub_ctx, eval_ctx);
        if (!sub_ctx.selectors.empty())
        {
            assert(sub_ctx.operators.empty());

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
            new_sel->repeat = true;
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

    case cgac::OP_CONST:
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

void RuleLoader::Context::
FlushOperators(const RulePtr& rule)
{
    assert(rule && selectors.empty());

    // flush operators
    if (!operators.empty()) {
        for (auto& op : operators) {
            rule->AddOperator(op);
        }
        operators.clear();
    }
}

void RuleLoader::Context::
FlushSelectors(bool repeat)
{
    if (!operators.empty() && !selectors.empty())
    {
        auto& op = operators.back();
        assert(op->selectors.sels.empty());
        if (selectors.size() == 1 && selectors[0]->GetType() == Rule::Selector::Type::Compound)
        {
            auto comp_sel = std::static_pointer_cast<Rule::CompoundSel>(selectors[0]);
            op->selectors.repeat = comp_sel->repeat;
            op->selectors.sels = comp_sel->sels;
        }
        else
        {
            op->selectors.repeat = repeat;
            std::copy(selectors.begin(), selectors.end(), std::back_inserter(op->selectors.sels));
        }
        selectors.clear();
    }
}

}