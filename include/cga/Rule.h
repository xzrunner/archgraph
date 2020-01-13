#pragma once

#include "cga/typedef.h"

#include <cgac/typedef.h>

#include <vector>
#include <memory>
#include <map>

namespace cga
{

class Operation;
class EvalContext;

class Rule
{
public:
    struct Operator;
    using OpPtr = std::shared_ptr<Operator>;

    struct Selector
    {
        enum class Type
        {
            Single,
            Compound,
        };

        virtual Type GetType() const = 0;

        bool duplicate = false;
    };

    using SelPtr = std::shared_ptr<Selector>;

    struct SingleSel : public Selector
    {
        virtual Type GetType() const override { return Type::Single; }

        cgac::ExprNodePtr head = nullptr;
        std::vector<OpPtr> ops;
    };

    struct CompoundSel : public Selector
    {
        virtual Type GetType() const override { return Type::Compound; }

        std::vector<SelPtr> sels;
    };

    enum class OpType
    {
        Unknown = 0,

        Rule,
        Operation,
        Func,
    };

    struct Operator
    {
        std::string name;
        std::vector<cgac::ExprNodePtr> params;
        CompoundSel selectors;

        OpType type = OpType::Unknown;

        std::weak_ptr<Rule>       rule;
        std::shared_ptr<Operation> op   = nullptr;
        FuncPtr                    func = nullptr;

        void Deduce(const std::map<std::string, RulePtr>& rules, const EvalContext& ctx);
    };

public:
    Rule(const std::string& name, const std::vector<std::string>& params = std::vector<std::string>());

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