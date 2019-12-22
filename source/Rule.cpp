#include "cga/Rule.h"
#include "cga/Node.h"

#include <rttr/type.h>

#include <assert.h>

namespace cga
{

Rule::Rule(const std::string& name)
    : m_name(name)
{
}

void Rule::AddOperator(const OpPtr& op)
{
    m_ops.push_back(op);
}

//////////////////////////////////////////////////////////////////////////
// struct Rule::Operator
//////////////////////////////////////////////////////////////////////////

void Rule::Operator::Deduce(const std::map<std::string, cgac::ExprNodePtr>& symbols,
                            const std::map<std::string, RulePtr>& rules)
{
    // rule
    auto itr = rules.find(name);
    if (itr != rules.end())
    {
        type = Rule::OpType::Rule;
        rule = itr->second;
        if (!rule.lock()) {
            int zz = 0;
        }
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

        std::vector<cgac::ExprNodePtr> _params, _selectors;
        _params.reserve(params.size());
        for (auto& p : params) {
            _params.push_back(p.expr);
        }
        _selectors.reserve(selectors.size());
        for (auto& s : selectors) {
            _selectors.push_back(s->head);
        }
        node->Setup(_params, _selectors, symbols);

        return;
    }

    assert(0);
}

}