#include "ce/EvalOp.h"
#include "ce/EvalRule.h"
#include "ce/op/Split.h"

#include <dag/Evaluator.h>

namespace
{

std::shared_ptr<ce::Rule>
create_rule(ce::EvalRule& rule_eval, size_t idx,
            const std::vector<ce::OpPtr>& ops_sorted,
            std::vector<bool>& load_flags)
{
    assert(idx >= 0 && idx < ops_sorted.size() && ops_sorted.size() == load_flags.size() && !load_flags[idx]);
    auto rule = std::make_shared<ce::Rule>("rule" + std::to_string(idx));
    rule_eval.AddRule(rule);

    auto op_node = ops_sorted[idx];

    auto op = std::make_shared<ce::Rule::Operator>();
    op->type = ce::Rule::OpType::Operation;
    op->op   = op_node;
    rule->AddOperator(op);

    // repeat
    if (op_node->get_type() == rttr::type::get<ce::op::Split>()) {
        auto split = std::static_pointer_cast<ce::op::Split>(op_node);
        op->selectors.repeat = split->GetRepeat();
    }

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
                size_t sub_idx = ops_sorted.size();

                auto& conn = out.conns[0];
                auto out_op = conn.node.lock();
                for (size_t i = 0, n = ops_sorted.size(); i < n; ++i) {
                    if (ops_sorted[i] == out_op) {
                        sub_idx = i;
                        break;
                    }
                }

                //auto sub_sel = std::make_shared<ce::Rule::SingleSel>();
                //auto sub_op = std::make_shared<ce::Rule::Operator>();
                //sub_op->node = nodes_sorted[sub_idx].first;
                //sub_op->type = ce::Rule::OpType::Operation;
                //sub_sel->ops.push_back(sub_op);
                //op->selectors.sels.push_back(sub_sel);
                //load_flags[sub_idx] = true;

                assert(sub_idx < ops_sorted.size() && !load_flags[sub_idx]);
                auto sub_rule = create_rule(rule_eval, sub_idx, ops_sorted, load_flags);
                auto sub_sel = std::make_shared<ce::Rule::SingleSel>();
                auto sub_op = std::make_shared<ce::Rule::Operator>();
                sub_op->rule = sub_rule;
                sub_op->type = ce::Rule::OpType::Rule;
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

namespace ce
{

std::shared_ptr<EvalRule>
EvalOp::ToRule(const EvalContext& ctx) const
{
    if (m_ops_map.empty()) {
        return nullptr;
    }

    auto rule_eval = std::make_shared<ce::EvalRule>();

    std::vector<std::pair<OpPtr, void*>> pairs;
    pairs.reserve(m_ops_map.size());
    for (auto itr : m_ops_map) {
        pairs.push_back(itr.second);
    }
    std::vector<std::shared_ptr<dag::Node<OpVarType>>> ops;
    ops.reserve(pairs.size());
    for (auto pair : pairs) {
        ops.push_back(pair.first);
    }
    auto sorted_idx = dag::Evaluator::TopologicalSorting(ops);
    std::vector<ce::OpPtr> ops_sorted;
    ops_sorted.reserve(ops.size());
    for (auto& idx : sorted_idx) {
        auto& op = std::static_pointer_cast<Operation>(ops[idx]);
        ops_sorted.push_back(op);
    }

    std::vector<bool> load_flags(sorted_idx.size(), false);
    for (size_t i = 0, n = ops_sorted.size(); i < n; ++i)
    {
        if (load_flags[i]) {
            continue;
        }

        auto rule = create_rule(*rule_eval, i, ops_sorted, load_flags);
        assert(rule);
        rule_eval->AddRule(rule);
    }

    rule_eval->OnLoadFinished(ctx);

    return rule_eval;
}

}