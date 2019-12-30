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

    LoadStatement(eval, ast);

    FlushRule(eval);

    eval.OnLoadFinished();

    return true;
}

void RuleLoader::LoadStatement(EvalRule& eval, const cgac::StmtNodePtr& stmt)
{
    switch (stmt->kind)
    {
    case cgac::NK_ExpressionStatement:
        LoadExpression(eval, std::static_pointer_cast<cgac::ExprStmtNode>(stmt)->expr);
        break;

    case cgac::NK_CompoundStatement:
    {
        auto p = std::static_pointer_cast<cgac::CompoundStmtNode>(stmt)->stmts;
        while (p != NULL) {
            LoadStatement(eval, std::static_pointer_cast<cgac::StatementNode>(p));
            p = p->next;
        }
    }
        break;
    }
}

void RuleLoader::LoadExpression(EvalRule& eval, const cgac::ExprNodePtr& expr)
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
        LoadExpression(eval, expr->kids[0]);
        LoadExpression(eval, expr->kids[1]);
    }
        break;

    case cgac::OP_COLON:
    {
        assert(!m_curr_sel);
        m_curr_sel = std::make_shared<Rule::Selector>();

        m_curr_sel->head = expr->kids[0];

        LoadExpression(eval, expr->kids[1]);

        assert(m_curr_rule);
        auto& ops = m_curr_rule->GetAllOps();
        assert(!ops.empty());
        ops.back()->selectors.push_back(m_curr_sel);
        m_curr_sel.reset();
    }
        break;

    case cgac::OP_RULE:
    {
        FlushRule(eval);

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
        m_curr_rule = std::make_shared<Rule>(rule_name, rule_params);

        LoadExpression(eval, expr->kids[1]);
    }
        break;

    case cgac::OP_CALL:
    {
        auto op = std::make_shared<Rule::Operator>();

        assert(expr->kids[0]->op == cgac::OP_ID);
        op->name = (char*)(expr->kids[0]->val.p);

        auto p = expr->kids[1];
        while (p) {
            op->params.push_back(Rule::Param(p));
            p = std::static_pointer_cast<cgac::ExpressionNode>(p->next);
        }

        if (m_curr_sel) {
            m_curr_sel->ops.push_back(op);
        } else {
            assert(m_curr_rule);
            m_curr_rule->AddOperator(op);
        }
    }
        break;

    case cgac::OP_ID:
    {
        auto op = std::make_shared<Rule::Operator>();
        op->name = (char*)(expr->val.p);

        if (m_curr_sel) {
            m_curr_sel->ops.push_back(op);
        } else {
            assert(m_curr_rule);
            m_curr_rule->AddOperator(op);
        }
    }
        break;

    case cgac::OP_ATTR:
        break;

    default:
        assert(0);
    }
}

void RuleLoader::FlushRule(EvalRule& eval)
{
    assert(!m_curr_sel);
    if (m_curr_rule) {
        eval.AddRule(m_curr_rule);
        m_curr_rule.reset();
    }
}

}