#include "cga/EvalNode.h"
#include "cga/EvalRule.h"

namespace
{

std::shared_ptr<cga::Rule>
create_rule(cga::EvalRule& rule_eval, size_t idx,
            const std::vector<std::pair<cga::NodePtr, void*>>& nodes_sorted,
            std::vector<bool>& load_flags)
{
    assert(idx >= 0 && idx < nodes_sorted.size() && nodes_sorted.size() == load_flags.size() && !load_flags[idx]);
    auto rule = std::make_shared<cga::Rule>("rule" + std::to_string(idx));
    rule_eval.AddRule(rule);

    auto op_node = nodes_sorted[idx].first;

    auto op = std::make_shared<cga::Rule::Operator>();
    op->type = cga::Rule::OpType::Node;
    op->node = op_node;
    rule->AddOperator(op);

    if (!op_node->GetExports().empty())
    {
        for (auto& out : op_node->GetExports())
        {
            if (out.conns.empty())
            {
                op->selectors.sels.push_back(nullptr);
            }
            else
            {
                size_t sub_idx = nodes_sorted.size();

                auto& conn = out.conns[0];
                auto out_node = conn.node.lock();
                for (size_t i = 0, n = nodes_sorted.size(); i < n; ++i) {
                    if (nodes_sorted[i].first == out_node) {
                        sub_idx = i;
                        break;
                    }
                }

                //auto sub_sel = std::make_shared<cga::Rule::SingleSel>();
                //auto sub_op = std::make_shared<cga::Rule::Operator>();
                //sub_op->node = nodes_sorted[sub_idx].first;
                //sub_op->type = cga::Rule::OpType::Node;
                //sub_sel->ops.push_back(sub_op);
                //op->selectors.sels.push_back(sub_sel);
                //load_flags[sub_idx] = true;

                assert(sub_idx < nodes_sorted.size() && !load_flags[sub_idx]);
                auto sub_rule = create_rule(rule_eval, sub_idx, nodes_sorted, load_flags);
                auto sub_sel = std::make_shared<cga::Rule::SingleSel>();
                auto sub_op = std::make_shared<cga::Rule::Operator>();
                sub_op->rule = sub_rule;
                sub_op->type = cga::Rule::OpType::Rule;
                sub_sel->ops.push_back(sub_op);
                op->selectors.sels.push_back(sub_sel);
            }
        }
    }

    op->selectors.sels.reserve(op_node->GetExports().size());

    load_flags[idx] = true;

    return rule;
}

}

namespace cga
{

std::shared_ptr<EvalRule>
EvalNode::ToRule() const
{
    TopologicalSorting();

    if (m_nodes_sorted.empty()) {
        return nullptr;
    }

    auto rule_eval = std::make_shared<cga::EvalRule>();

    std::vector<bool> load_flags(m_nodes_sorted.size(), false);
    for (size_t i = 0, n = m_nodes_sorted.size(); i < n; ++i)
    {
        if (load_flags[i]) {
            continue;
        }

        auto rule = create_rule(*rule_eval, i, m_nodes_sorted, load_flags);
        assert(rule);
        rule_eval->AddRule(rule);
    }

    rule_eval->OnLoadFinished();

    return rule_eval;
}

}