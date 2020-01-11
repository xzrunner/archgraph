#include "cga/Rule.h"
#include "cga/Node.h"
#include "cga/CGA.h"

#include <rttr/type.h>

#include <assert.h>

namespace cga
{

Rule::Rule(const std::string& name, const std::vector<std::string>& params)
    : m_name(name)
    , m_params(params)
{
}

void Rule::AddOperator(const OpPtr& op)
{
    m_ops.push_back(op);
}

//////////////////////////////////////////////////////////////////////////
// struct Rule::Operator
//////////////////////////////////////////////////////////////////////////

void Rule::Operator::Deduce(const std::map<std::string, RulePtr>& rules, const EvalContext& ctx)
{
    if (rule.lock() || node) {
        return;
    }

    // rule
    auto itr_rule = rules.find(name);
    if (itr_rule != rules.end())
    {
        type = Rule::OpType::Rule;
        rule = itr_rule->second;
        return;
    }

    // func
    auto find_func = CGA::Instance()->QueryFunc(name);
    if (find_func)
    {
        type = Rule::OpType::Func;
        func = find_func;
        return;
    }

    // node
    auto rttr_type = rttr::type::get_by_name("cga::" + name);
    if (rttr_type.is_valid())
    {
        type = Rule::OpType::Node;

        rttr::variant var = rttr_type.create();
        assert(var.is_valid());

        node = var.get_value<std::shared_ptr<cga::Node>>();
        assert(node);

        node->Setup(params, selectors, ctx);

        return;
    }    

    assert(0);
}

}