#pragma once

#include "cga/typedef.h"

#include <cgac/typedef.h>

#include <vector>
#include <memory>
#include <map>

namespace cga
{

class Node;

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

        void Deduce(const std::map<std::string, cgac::ExprNodePtr>& symbols,
            const std::map<std::string, RulePtr>& rules);

    };

public:
    Rule(const std::string& name);

    auto& GetName() const { return m_name; }

    auto& GetAllOps() const { return m_ops; }

    void AddOperator(const OpPtr& op);

private:
    std::string m_name;

    std::vector<OpPtr> m_ops;

}; // Rule

}