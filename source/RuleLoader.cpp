#include "cga/RuleLoader.h"

#include <cgac/Parser.h>
#include <cgac/Statement.h>
#include <cgac/DumpAST.h>
#include <cgac/Statement.h>

#include <iostream>

#define EVAL_V0                              \
auto v0 = EvalExpression(expr->kids[0]);     \
if (v0.type == VarType::Invalid) {           \
    return Variant(VarType::Invalid);        \
}

#define EVAL_V1                              \
auto v1 = EvalExpression(expr->kids[1]);     \
if (v1.type == VarType::Invalid) {           \
    return Variant(VarType::Invalid);        \
}

namespace cga
{

RuleLoader::RuleLoader()
{
    cgac::SetupTypeSystem();
}

void RuleLoader::RunString(const std::string& str, Evaluator& eval)
{
    if (str.empty()) {
        return;
    }

    const std::string s = "{" + str + "}";
    cgac::Parser parser(s.c_str());
    auto ast = cgac::StatementParser::ParseStatement(parser);

    cgac::DumpStatement(std::cout, ast, 0);

    LoadStatement(ast);

    FlushRule();

    TransToNodeGraph(eval);
}

void RuleLoader::LoadStatement(const std::shared_ptr<cgac::StatementNode>& stmt)
{
    switch (stmt->kind)
    {
    case cgac::NK_SymbolStatement:
    {
        FlushRule();
        LoadExpression(std::static_pointer_cast<cgac::SymbolStmtNode>(stmt)->expr);
    }
        break;

    case cgac::NK_ExpressionStatement:
        LoadExpression(std::static_pointer_cast<cgac::ExprStmtNode>(stmt)->expr);
        break;

    case cgac::NK_CompoundStatement:
    {
        auto p = std::static_pointer_cast<cgac::CompoundStmtNode>(stmt)->stmts;
        while (p != NULL) {
            LoadStatement(std::static_pointer_cast<cgac::StatementNode>(p));
            p = p->next;
        }
    }
        break;

    case cgac::NK_RuleStatement:
    {
        FlushRule();

        m_curr_rule = std::make_shared<Rule>();

        auto rule_stmt = std::static_pointer_cast<cgac::RuleStmtNode>(stmt);
        m_curr_rule->name = rule_stmt->rule;
        LoadStatement(rule_stmt->stmt);
    }
        break;
    }
}

void RuleLoader::LoadExpression(const std::shared_ptr<cgac::ExpressionNode>& expr)
{
    switch (expr->op)
    {
    case cgac::OP_ASSIGN:
    {
        switch (expr->kids[0]->op)
        {
        case cgac::OP_ID:
            m_symbols.insert({ (char*)(expr->kids[0]->val.p), EvalExpression(expr->kids[1]) });
            break;

        default:
            assert(0);
        }
    }
        break;

    case cgac::OP_SEPARATOR:
    {
        LoadExpression(expr->kids[0]);
        LoadExpression(expr->kids[1]);
    }
        break;

    case cgac::OP_COLON:
    {
        assert(!m_curr_sel);
        m_curr_sel = std::make_shared<Selector>();

        LoadExpression(expr->kids[0]);
        LoadExpression(expr->kids[1]);

        assert(m_curr_rule && !m_curr_rule->actions.empty());
        m_curr_rule->actions.back()->selectors.push_back(m_curr_sel);
        m_curr_sel.reset();
    }
        break;

    case cgac::OP_CALL:
    {
        auto action = std::make_shared<Action>();

        assert(expr->kids[0]->op == cgac::OP_ID);
        action->name = (char*)(expr->kids[0]->val.p);

        auto p = expr->kids[1];
        while (p) {
            action->params.push_back(EvalExpression(p));
            p = std::static_pointer_cast<cgac::ExpressionNode>(p->next);
        }

        if (m_curr_sel) {
            m_curr_sel->actions.push_back(action);
        } else {
            assert(m_curr_rule);
            m_curr_rule->actions.push_back(action);
        }
    }
        break;

    case cgac::OP_ID:
    {
        auto action = std::make_shared<Action>();
        action->name = (char*)(expr->val.p);

        if (m_curr_sel) {
            m_curr_sel->actions.push_back(action);
        } else {
            assert(m_curr_rule);
            m_curr_rule->actions.push_back(action);
        }
    }
        break;

    default:
        assert(0);
    }
}

RuleLoader::Variant
RuleLoader::EvalExpression(const std::shared_ptr<cgac::ExpressionNode>& expr) const
{
    switch (expr->op)
    {
    case cgac::OP_COMMA:
        return EvalExpression(expr->kids[1]);
    case cgac::OP_ASSIGN:
        return EvalExpression(expr->kids[1]);
    case cgac::OP_OR:
    {
        EVAL_V0
        EVAL_V1
        return Variant(v0.ToBool() || v1.ToBool());
    }
    case cgac::OP_AND:
    {
        EVAL_V0
        EVAL_V1
        return Variant(v0.ToBool() && v1.ToBool());
    }
    case cgac::OP_EQUAL:
    {
        EVAL_V0
        EVAL_V1
        return Variant(abs(v0.ToFloat() - v1.ToFloat()) < std::numeric_limits<double>::epsilon());
    }
    case cgac::OP_UNEQUAL:
    {
        EVAL_V0
        EVAL_V1
        return Variant(abs(v0.ToFloat() - v1.ToFloat()) >= std::numeric_limits<double>::epsilon());
    }
    case cgac::OP_GREAT:
    {
        EVAL_V0
        EVAL_V1
        return Variant(v0.ToFloat() > v1.ToFloat());
    }
    case cgac::OP_LESS:
    {
        EVAL_V0
        EVAL_V1
        return Variant(v0.ToFloat() < v1.ToFloat());
    }
     case cgac::OP_GREAT_EQ:
    {
        EVAL_V0
        EVAL_V1
        return Variant(v0.ToFloat() >= v1.ToFloat());
    }
    case cgac::OP_LESS_EQ:
    {
        EVAL_V0
        EVAL_V1
        return Variant(v0.ToFloat() <= v1.ToFloat());
    }
    case cgac::OP_ADD:
    {
        EVAL_V0
        EVAL_V1
        if (v0.type == VarType::Float || v1.type == VarType::Float) {
            return Variant(v0.ToFloat() + v1.ToFloat());
        } else {
            return Variant();
        }
    }
    case cgac::OP_SUB:
    {
        EVAL_V0
        EVAL_V1
        if (v0.type == VarType::Float || v1.type == VarType::Float) {
            return Variant(v0.ToFloat() - v1.ToFloat());
        } else {
            return Variant();
        }
    }
    case cgac::OP_MUL:
    {
        EVAL_V0
        EVAL_V1
        if (v0.type == VarType::Float || v1.type == VarType::Float) {
            return Variant(v0.ToFloat() * v1.ToFloat());
        } else {
            return Variant();
        }
    }
    case cgac::OP_DIV:
    {
        EVAL_V0
        EVAL_V1
        auto d1 = v1.ToFloat();
        if (abs(d1) < std::numeric_limits<double>::epsilon()) {
            return Variant();
        }
        if (v0.type == VarType::Float || v1.type == VarType::Float) {
            return Variant(v0.ToFloat() / v1.ToFloat());
        } else {
            return Variant();
        }
    }
    case cgac::OP_MOD:
    {
        EVAL_V0
        EVAL_V1
        if (v0.type == VarType::Float && v1.type == VarType::Float) {
            auto mod = static_cast<int>(v0.f) % static_cast<int>(v1.f);
            return Variant(static_cast<float>(mod));
        } else {
            return Variant();
        }
    }
    case cgac::OP_NEG:
    {
        EVAL_V0
        switch (v0.type)
        {
        case VarType::Float:
            return Variant(-v0.f);
        default:
            assert(0);
            return Variant();
        }
    }
    case cgac::OP_NOT:
    {
        EVAL_V0
        return Variant(!v0.ToBool());
    }
    case cgac::OP_ID:
    {
        //if (GETTER) {
        //    auto var = GETTER((char*)(expr->val.p), ud);
        //    if (var.type != VarType::Invalid) {
        //        return var;
        //    }
        //}

        auto str = (char*)(expr->val.p);
        if (strcmp(str, "true") == 0) {
            return Variant(true);
        } else if (strcmp(str, "false") == 0) {
            return Variant(false);
        } else {
            return Variant();
        }
    }
    case cgac::OP_CONST:
    {
        int categ = expr->ty->categ;
        if (categ == cgac::FLOAT) {
            return Variant(expr->val.f);
        } else {
            assert(0);
            return Variant();
        }
    }
    case cgac::OP_STR:
    {
        return Variant(VarType::String, expr->val.p);
    }
    default:
        assert(0);
        return Variant();
    }
}

void RuleLoader::FlushRule()
{
    if (m_curr_rule)
    {
        m_rules.insert({ m_curr_rule->name, m_curr_rule });
        m_curr_rule.reset();
    }
}

void RuleLoader::TransToNodeGraph(Evaluator& eval)
{
    for (auto& rule : m_rules)
    {
        for (auto& action : rule.second->actions)
        {
            auto itr = m_rules.find(action->name);
            if (itr != m_rules.end())
            {

            }
            else
            {
                // rttr
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////
// class RuleLoader::Variant
//////////////////////////////////////////////////////////////////////////

RuleLoader::Variant::Variant()
    : type(VarType::Invalid)
{
}

RuleLoader::Variant::Variant(VarType type)
    : type(type)
{
}

RuleLoader::Variant::Variant(float f)
    : type(VarType::Float)
    , f(f)
{
}

RuleLoader::Variant::Variant(VarType type, const void* p)
    : type(type)
    , p(p)
{
}

bool RuleLoader::Variant::ToBool() const
{
    switch (type)
    {
    case VarType::Float:
        return fabs(f) > std::numeric_limits<float>::epsilon();
    case VarType::String:
        return true;
    default:
        assert(0);
        return false;
    }
}

float RuleLoader::Variant::ToFloat() const
{
    switch (type)
    {
    case VarType::Float:
        return f;
    default:
        assert(0);
        return 0;
    }
}

}