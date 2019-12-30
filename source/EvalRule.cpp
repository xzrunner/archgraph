#include "cga/EvalRule.h"
#include "cga/Rule.h"
#include "cga/Node.h"
#include "cga/EvalContext.h"
#include "cga/EvalExpr.h"
#include "cga/EvalHelper.h"

#include <stack>

#include <assert.h>

namespace cga
{

void EvalRule::AddRule(const RulePtr& rule)
{
    m_rules.insert({ rule->GetName(), rule });
}

void EvalRule::AddSymbol(const std::string& name,
                         const cgac::ExprNodePtr& val)
{
    m_ctx.AddGlobalParm({ name, val });
}

void EvalRule::OnLoadFinished()
{
    DeduceOps();
    TopologicalSorting();
}

std::vector<GeoPtr>
EvalRule::Eval(const std::vector<GeoPtr>& geos) const
{
    if (m_rules_sorted.empty()) {
        return std::vector<GeoPtr>();
    } else {
        auto rule = m_rules_sorted.front();
        return Eval(geos, rule->GetAllOps());
    }
}

void EvalRule::DeduceOps()
{
    for (auto& itr_rule: m_rules) {
        for (auto& op : itr_rule.second->GetAllOps()) {
            op->Deduce(m_rules, m_ctx);
            for (auto& sel : op->selectors) {
                for (auto& op : sel->ops) {
                    op->Deduce(m_rules, m_ctx);
                }
            }
        }
    }
}

void EvalRule::TopologicalSorting() const
{
    std::vector<RulePtr> rules;
    rules.reserve(m_rules.size());
    for (auto itr : m_rules) {
        rules.push_back(itr.second);
    }

    // prepare
    std::vector<int> in_deg(m_rules.size(), 0);
    std::vector<std::vector<int>> out_nodes(rules.size());
    for (int i = 0, n = rules.size(); i < n; ++i)
    {
        auto& rule = rules[i];
        for (auto& op : rule->GetAllOps())
        {
            auto child = op->rule.lock();
            if (!child) {
                continue;
            }

            for (int j = 0, m = rules.size(); j < m; ++j) {
                if (child == rules[j]) {
                    in_deg[i]++;
                    out_nodes[j].push_back(i);
                    break;
                }
            }
        }
    }

    // sort
    std::stack<int> st;
    m_rules_sorted.clear();
    for (int i = 0, n = in_deg.size(); i < n; ++i) {
        if (in_deg[i] == 0) {
            st.push(i);
        }
    }
    while (!st.empty())
    {
        int v = st.top();
        st.pop();
        m_rules_sorted.push_back(rules[v]);
        for (auto& i : out_nodes[v]) {
            assert(in_deg[i] > 0);
            in_deg[i]--;
            if (in_deg[i] == 0) {
                st.push(i);
            }
        }
    }
    std::reverse(m_rules_sorted.begin(), m_rules_sorted.end());
}

std::vector<GeoPtr>
EvalRule::Eval(const std::vector<GeoPtr>& geos, const std::vector<Rule::OpPtr>& ops) const
{
    std::vector<GeoPtr> curr = geos;
    for (auto& op : ops)
    {
        switch (op->type)
        {
        case Rule::OpType::Rule:
        {
            auto rule = op->rule.lock();
            assert(rule);

            std::vector<EvalContext::Parm> parms;
            assert(rule->GetParams().size() == op->params.size());
            parms.reserve(op->params.size());
            for (size_t i = 0, n = op->params.size(); i < n; ++i) {
                parms.push_back({ rule->GetParams()[i], op->params[i].expr });
            }
            m_ctx.SetLocalParms(parms);
            curr = Eval(curr, rule->GetAllOps());
        }
            break;
        case Rule::OpType::Node:
        {
            ResolveParmsExpr(*op->node);

            std::vector<GeoPtr> dst;
            for (auto& geo : curr) {
                if (geo) {
                    op->node->Execute({ geo }, dst, m_ctx);
                }
            }

            if (dst.empty())
            {
                ;
            }
            else if (dst.size() == 1)
            {
                assert(op->selectors.empty());
            }
            else
            {
                assert(dst.size() == op->selectors.size());
                for (size_t i = 0, n = dst.size(); i < n; ++i)
                {
                    std::vector<GeoPtr> src_geos, dst_geos;
                    src_geos.push_back(dst[i]);
                    dst_geos = Eval(src_geos, op->selectors[i]->ops);
                    if (dst_geos.empty()) {
                        dst[i].reset();
                    } else {
                        assert(dst_geos.size() == 1);
                        dst[i] = dst_geos[0];
                    }
                }
            }
            curr = dst;
        }
            break;
        case Rule::OpType::Func:
            break;
        default:
            assert(0);
        }
    }
    return curr;
}

void EvalRule::ResolveParmsExpr(Node& node) const
{
    auto& exprs = node.GetExprsMap();
    if (exprs.empty()) {
        return;
    }

    auto type = node.get_type();
    for (auto& expr : exprs)
    {
        auto prop = type.get_property(expr.first);
        if (!prop.is_valid()) {
            continue;
        }

        auto& parms = m_ctx.GetLocalParms();
        for (auto& p : parms)
        {
            if (p.name != expr.second) {
                continue;
            }

            if (p.val.type != dag::VarType::Invalid)
            {
                switch (p.val.type)
                {
                case dag::VarType::Float:
                    EvalHelper::SetPropVal(prop, node, EvalExpr::Variant(p.val.f));
                    break;
                case dag::VarType::String:
                    EvalHelper::SetPropVal(prop, node, EvalExpr::Variant(EvalExpr::VarType::String, p.val.p));
                    break;
                default:
                    assert(0);
                }
            }
            else
            {
                assert(p.val_expr);
                auto var = EvalExpr::Eval(p.val_expr);
                EvalHelper::SetPropVal(prop, node, EvalExpr::Variant(var));
            }

            break;
        }
    }
}

}