#include "cga/EvalOp.h"
#include "cga/EvalRule.h"
#include "cga/op/Split.h"

namespace
{

std::shared_ptr<cga::Rule>
create_rule(cga::EvalRule& rule_eval, size_t idx,
            const std::vector<std::pair<cga::OpPtr, void*>>& ops_sorted,
            std::vector<bool>& load_flags)
{
    assert(idx >= 0 && idx < ops_sorted.size() && ops_sorted.size() == load_flags.size() && !load_flags[idx]);
    auto rule = std::make_shared<cga::Rule>("rule" + std::to_string(idx));
    rule_eval.AddRule(rule);

    auto op_node = ops_sorted[idx].first;

    auto op = std::make_shared<cga::Rule::Operator>();
    op->type = cga::Rule::OpType::Operation;
    op->op   = op_node;
    rule->AddOperator(op);

    // repeat
    if (op_node->get_type() == rttr::type::get<cga::op::Split>()) {
        auto split = std::static_pointer_cast<cga::op::Split>(op_node);
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
                    if (ops_sorted[i].first == out_op) {
                        sub_idx = i;
                        break;
                    }
                }

                //auto sub_sel = std::make_shared<cga::Rule::SingleSel>();
                //auto sub_op = std::make_shared<cga::Rule::Operator>();
                //sub_op->node = nodes_sorted[sub_idx].first;
                //sub_op->type = cga::Rule::OpType::Operation;
                //sub_sel->ops.push_back(sub_op);
                //op->selectors.sels.push_back(sub_sel);
                //load_flags[sub_idx] = true;

                assert(sub_idx < ops_sorted.size() && !load_flags[sub_idx]);
                auto sub_rule = create_rule(rule_eval, sub_idx, ops_sorted, load_flags);
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
EvalOp::ToRule() const
{
    TopologicalSorting();

    if (m_ops_sorted.empty()) {
        return nullptr;
    }

    auto rule_eval = std::make_shared<cga::EvalRule>();

    std::vector<bool> load_flags(m_ops_sorted.size(), false);
    for (size_t i = 0, n = m_ops_sorted.size(); i < n; ++i)
    {
        if (load_flags[i]) {
            continue;
        }

        auto rule = create_rule(*rule_eval, i, m_ops_sorted, load_flags);
        assert(rule);
        rule_eval->AddRule(rule);
    }

    rule_eval->OnLoadFinished();

    return rule_eval;
}

}