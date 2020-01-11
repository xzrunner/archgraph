#include "cga/EvalRule.h"
#include "cga/Rule.h"
#include "cga/Node.h"
#include "cga/EvalContext.h"
#include "cga/Variant.h"
#include "cga/EvalHelper.h"
#include "cga/Geometry.h"
#include "cga/EvalExpr.h"
#include "cga/Function.h"

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

RulePtr EvalRule::QueryRule(const std::string& name) const
{
    auto itr = m_rules.find(name);
    return itr == m_rules.end() ? nullptr : itr->second;
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

void EvalRule::Clear()
{
    m_filepath.clear();
    m_rules.clear();
    m_rules.clear();
    m_ctx.Clear();
}

void EvalRule::DeduceOps()
{
    for (auto& itr_rule: m_rules) {
        for (auto& op : itr_rule.second->GetAllOps()) {
            op->Deduce(m_rules, m_ctx);
            for (auto& sel : op->selectors.sels) {
                DeduceOps(sel);
            }
        }
    }
}

void EvalRule::DeduceOps(const Rule::SelPtr& sel)
{
    if (!sel) {
        return;
    }
    switch (sel->GetType())
    {
    case Rule::Selector::Type::Single:
    {
        auto single_sel = std::static_pointer_cast<Rule::SingleSel>(sel);
        for (auto& op : single_sel->ops) {
            op->Deduce(m_rules, m_ctx);
        }
    }
        break;
    case Rule::Selector::Type::Compound:
    {
        auto comp_sel = std::static_pointer_cast<Rule::CompoundSel>(sel);
        for (auto& csel : comp_sel->sels) {
            DeduceOps(csel);
        }
    }
        break;
    default:
        assert(0);
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
                parms.push_back({ rule->GetParams()[i], op->params[i] });
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

            if (dst.empty()) {
                ;
            } else if (dst.size() == 1 && op->selectors.sels.empty()) {
                ;
            } else {
                dst = Eval(dst, op->selectors);
            }
            curr = dst;
        }
            break;
        case Rule::OpType::Func:
        {
            std::vector<VarPtr> parms;
            parms.reserve(op->params.size());
            for (auto& p : op->params) {
                parms.push_back(EvalExpr::Eval(p));
            }
            assert(op->func);
            op->func->Eval(parms, curr, m_console);
        }
            break;
        default:
            assert(0);
        }
    }
    return curr;
}

std::vector<GeoPtr>
EvalRule::Eval(const std::vector<GeoPtr>& geos, const Rule::CompoundSel& comp_sel) const
{
    std::vector<GeoPtr> ret;
    assert(geos.size() == comp_sel.sels.size()
        || (geos.size() > comp_sel.sels.size() && comp_sel.duplicate && (geos.size() % comp_sel.sels.size() == 0)));
    for (size_t i = 0, n = geos.size(); i < n; ++i)
    {
        auto& sel = comp_sel.sels[i % comp_sel.sels.size()];
        if (!sel) {
            ret.push_back(geos[i]);
            continue;
        }

        std::vector<GeoPtr> src_geos, dst_geos;
        src_geos.push_back(geos[i]);

        switch (sel->GetType())
        {
        case Rule::Selector::Type::Single:
        {
            auto single_sel = std::static_pointer_cast<Rule::SingleSel>(sel);
            dst_geos = Eval(src_geos, single_sel->ops);
        }
            break;
        case Rule::Selector::Type::Compound:
        {
            auto& children = geos[i]->GetChildren();
            assert(!children.empty());
            auto comp_sel = std::static_pointer_cast<Rule::CompoundSel>(sel);
            dst_geos = Eval(children, *comp_sel);
        }
            break;
        default:
            assert(0);
        }

        if (dst_geos.empty()) {
            ret.push_back(nullptr);
        } else {
            std::copy(dst_geos.begin(), dst_geos.end(), std::back_inserter(ret));
        }
    }
    return ret;
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
                    EvalHelper::SetPropVal(prop, node, std::make_unique<FloatVar>(p.val.f));
                    break;
                case dag::VarType::String:
                    EvalHelper::SetPropVal(prop, node, std::make_unique<StringVar>(static_cast<const char*>(p.val.p)));
                    break;
                default:
                    assert(0);
                }
            }
            else
            {
                assert(p.val_expr);
                auto var = EvalExpr::Eval(p.val_expr);
                EvalHelper::SetPropVal(prop, node, var);
            }

            break;
        }
    }
}

}