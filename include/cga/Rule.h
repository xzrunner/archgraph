#pragma once

#include "cga/typedef.h"

#include <cgac/typedef.h>

#include <vector>
#include <memory>
#include <map>

namespace cga
{

class Node;
class EvalContext;

class Rule
{
public:
    struct Param
    {
        Param(const cgac::ExprNodePtr& expr)
            : expr(expr)
        {
        }

        cgac::ExprNodePtr expr = nullptr;
    };

    struct Operator;
    using OpPtr = std::shared_ptr<Operator>;

    struct Selector
    {
        cgac::ExprNodePtr head = nullptr;
        std::vector<OpPtr> ops;
    };

    using SelPtr = std::shared_ptr<Selector>;

    enum class OpType
    {
        Unknown = 0,

        Rule,
        Node,
        Func,
    };

    struct Operator
    {
        std::string         name;
        std::vector<Param>  params;
        std::vector<SelPtr> selectors;

        OpType type = OpType::Unknown;

        std::weak_ptr<Rule>   rule;
        std::shared_ptr<Node> node = nullptr;

        void Deduce(const std::map<std::string, RulePtr>& rules, const EvalContext& ctx);
    };

public:
    Rule(const std::string& name, const std::vector<std::string>& params);

    auto& GetName() const { return m_name; }
    auto& GetParams() const { return m_params; }

    auto& GetAllOps() const { return m_ops; }

    void AddOperator(const OpPtr& op);

private:
    std::string m_name;
    std::vector<std::string> m_params;

    std::vector<OpPtr> m_ops;

}; // Rule

}