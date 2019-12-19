#pragma once

#include <string>
#include <map>
#include <vector>

namespace cgac { struct ExpressionNode; struct StatementNode; }

namespace cga
{

class Evaluator;

class RuleLoader
{
public:
    RuleLoader();

    void RunString(const std::string& str, Evaluator& eval);

private:
    enum class VarType
    {
        Invalid = 0,

        Float,
        String,
    };

    struct Variant
    {
        Variant();
        Variant(VarType type);
        explicit Variant(float f);
        explicit Variant(VarType type, const void* p);

        bool ToBool() const;
        float ToFloat() const;

        VarType type = VarType::Invalid;

        union
        {
            float       f;
            const void* p;
        };
    };

    struct Action;
    struct Selector
    {
        std::vector<std::shared_ptr<Action>> actions;
    };

    struct Action
    {
        std::string name;
        std::vector<Variant> params;

        std::vector<std::shared_ptr<Selector>> selectors;
    };

    struct Rule
    {
        std::string name;

        std::vector<std::shared_ptr<Action>> actions;
    };

private:
    void LoadStatement(const std::shared_ptr<cgac::StatementNode>& stmt);
    void LoadExpression(const std::shared_ptr<cgac::ExpressionNode>& expr);

    Variant EvalExpression(const std::shared_ptr<cgac::ExpressionNode>& expr) const;

    void FlushRule();

    void TransToNodeGraph(Evaluator& eval);

private:
    std::map<std::string, Variant> m_symbols;

    std::map<std::string, std::shared_ptr<Rule>> m_rules;

    std::shared_ptr<Rule> m_curr_rule = nullptr;
    std::shared_ptr<Selector> m_curr_sel = nullptr;

}; // RuleLoader

}