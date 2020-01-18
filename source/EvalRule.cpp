#include "cga/EvalRule.h"
#include "cga/Rule.h"
#include "cga/Operation.h"
#include "cga/EvalContext.h"
#include "cga/Variant.h"
#include "cga/EvalHelper.h"
#include "cga/Geometry.h"
#include "cga/EvalExpr.h"
#include "cga/Function.h"

#include <stack>
#include <queue>

#include <assert.h>

namespace cga
{

void EvalRule::AddRule(const RulePtr& rule)
{
    m_rules.insert({ rule->GetName(), rule });
}

RulePtr EvalRule::QueryRule(const std::string& name) const
{
    auto itr = m_rules.find(name);
    return itr == m_rules.end() ? nullptr : itr->second;
}

void EvalRule::OnLoadFinished(const EvalContext& ctx)
{
    DeduceOps(ctx);

    TopologicalSorting sort(m_rules);
    m_rules_sorted = sort.Sort();
}

std::vector<GeoPtr>
EvalRule::Eval(const std::vector<GeoPtr>& geos, const EvalContext& ctx)
{
    if (m_rules_sorted.empty()) {
        return std::vector<GeoPtr>();
    } else {
        auto rule = m_rules_sorted.front();
        return Eval(geos, rule->GetAllOps(), ctx);
    }
}

void EvalRule::Clear()
{
    m_filepath.clear();
    m_rules.clear();
    m_rules.clear();
}

void EvalRule::DeduceOps(const EvalContext& ctx)
{
    for (auto& itr_rule: m_rules) {
        for (auto& op : itr_rule.second->GetAllOps()) {
            op->Deduce(m_rules, ctx);
            for (auto& sel : op->selectors.sels) {
                DeduceOps(ctx, sel);
            }
        }
    }
}

void EvalRule::DeduceOps(const EvalContext& ctx, const Rule::SelPtr& sel)
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
            op->Deduce(m_rules, ctx);
        }
    }
        break;
    case Rule::Selector::Type::Compound:
    {
        auto comp_sel = std::static_pointer_cast<Rule::CompoundSel>(sel);
        for (auto& csel : comp_sel->sels) {
            DeduceOps(ctx, csel);
        }
    }
        break;
    default:
        assert(0);
    }
}

std::vector<GeoPtr>
EvalRule::Eval(const std::vector<GeoPtr>& geos, const std::vector<Rule::OpPtr>& ops,
               const EvalContext& ctx)
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

            EvalContext new_ctx = ctx;
            assert(rule->GetParams().size() == op->params.size());
            for (size_t i = 0, n = op->params.size(); i < n; ++i) {
                new_ctx.AddVar({ rule->GetParams()[i], op->params[i] });
            }
            curr = Eval(curr, rule->GetAllOps(), new_ctx);
        }
            break;
        case Rule::OpType::Operation:
        {
            ResolveParmsExpr(*op->op, ctx);

            std::vector<GeoPtr> dst;
            if (curr.empty())
            {
                op->op->Execute({}, dst, ctx);
            }
            else
            {
                for (auto& geo : curr) {
                    if (geo) {
                        op->op->Execute({ geo }, dst, m_ctx);
                    }
                }
            }

            if (dst.empty()) {
                ;
            } else if (dst.size() == 1 && op->selectors.sels.empty()) {
                ;
            } else {
                dst = Eval(dst, op->selectors, ctx);
            }
            curr = dst;
        }
            break;
        case Rule::OpType::Func:
        {
            std::vector<VarPtr> parms;
            parms.reserve(op->params.size());
            for (auto& p : op->params) {
                parms.push_back(EvalExpr::Eval(p, ctx));
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
EvalRule::Eval(const std::vector<GeoPtr>& geos, const Rule::CompoundSel& comp_sel,
               const EvalContext& ctx)
{
    std::vector<GeoPtr> ret;
    assert(geos.size() == comp_sel.sels.size()
        || (geos.size() > comp_sel.sels.size() && comp_sel.repeat && (geos.size() % comp_sel.sels.size() == 0)));
    for (size_t i = 0, n = geos.size(); i < n; ++i)
    {
        auto& sel = comp_sel.sels[i % comp_sel.sels.size()];
        if (!sel)
        {
            if (geos[i]) {
                ret.push_back(geos[i]);
            }
            continue;
        }

        std::vector<GeoPtr> src_geos, dst_geos;
        src_geos.push_back(geos[i]);

        switch (sel->GetType())
        {
        case Rule::Selector::Type::Single:
        {
            auto single_sel = std::static_pointer_cast<Rule::SingleSel>(sel);
            dst_geos = Eval(src_geos, single_sel->ops, ctx);
        }
            break;
        case Rule::Selector::Type::Compound:
        {
            auto& children = geos[i]->GetChildren();
            assert(!children.empty());
            auto comp_sel = std::static_pointer_cast<Rule::CompoundSel>(sel);
            dst_geos = Eval(children, *comp_sel, ctx);
        }
            break;
        default:
            assert(0);
        }

        if (dst_geos.empty()) {
            //ret.push_back(nullptr);
        } else {
            //for (auto& geo : dst_geos) {
            //    if (geo) {
            //        ret.push_back(geo);
            //    }
            //}
            std::copy(dst_geos.begin(), dst_geos.end(), std::back_inserter(ret));
        }
    }
    return ret;
}

void EvalRule::ResolveParmsExpr(Operation& op, const EvalContext& ctx) const
{
    auto& exprs = op.GetExprsMap();
    if (exprs.empty()) {
        return;
    }

    auto type = op.get_type();
    for (auto& expr : exprs)
    {
        auto prop = type.get_property(expr.first);
        if (!prop.is_valid()) {
            continue;
        }

        auto parm = ctx.QueryVar(expr.second);
        if (!parm) {
            continue;
        }

        if (parm->value.type != dag::VarType::Invalid)
        {
            switch (parm->value.type)
            {
            case dag::VarType::Float:
                EvalHelper::SetPropVal(prop, op, std::make_unique<FloatVar>(parm->value.f));
                break;
            case dag::VarType::String:
                EvalHelper::SetPropVal(prop, op, std::make_unique<StringVar>(static_cast<const char*>(parm->value.p)));
                break;
            default:
                assert(0);
            }
        }
        else
        {
            assert(parm->expr);
            auto var = EvalExpr::Eval(parm->expr, ctx);
            EvalHelper::SetPropVal(prop, op, var);
        }
    }
}

//////////////////////////////////////////////////////////////////////////
// class EvalRule::TopologicalSorting
//////////////////////////////////////////////////////////////////////////

EvalRule::TopologicalSorting::
TopologicalSorting(const std::map<std::string, RulePtr>& rules)
{
    m_rules.reserve(rules.size());
    for (auto itr : rules) {
        m_rules.push_back(itr.second);
    }

    m_in_deg.resize(m_rules.size(), 0);
    m_out_ops.resize(m_rules.size());
}

std::vector<RulePtr> EvalRule::TopologicalSorting::
Sort()
{
    // prepare
    for (int i = 0, n = m_rules.size(); i < n; ++i)
    {
        auto& rule = m_rules[i];
        for (auto& op : rule->GetAllOps()) {
            PrepareOp(op, i);
        }
    }

    // sort
    std::vector<RulePtr> ret;
    std::stack<int> st;
    ret.clear();
    for (int i = 0, n = m_in_deg.size(); i < n; ++i) {
        if (m_in_deg[i] == 0) {
            st.push(i);
        }
    }
    while (!st.empty())
    {
        int v = st.top();
        st.pop();
        ret.push_back(m_rules[v]);
        for (auto& i : m_out_ops[v]) {
            assert(m_in_deg[i] > 0);
            m_in_deg[i]--;
            if (m_in_deg[i] == 0) {
                st.push(i);
            }
        }
    }
    std::reverse(ret.begin(), ret.end());

    return ret;
}

void EvalRule::TopologicalSorting::
PrepareOp(const Rule::OpPtr& op, int rule_idx)
{
    auto rule = op->rule.lock();
    if (rule) {
        AddRuleDepend(rule, rule_idx);
    }

    for (auto& sel : op->selectors.sels) {
        if (sel) {
            PrepareSel(sel, rule_idx);
        }
    }
}

void EvalRule::TopologicalSorting::
PrepareSel(const Rule::SelPtr& sel, int rule_idx)
{
    switch (sel->GetType())
    {
    case Rule::Selector::Type::Single:
    {
        auto s_sel = std::static_pointer_cast<Rule::SingleSel>(sel);
        for (auto& op : s_sel->ops) {
            PrepareOp(op, rule_idx);
        }
    }
        break;
    case Rule::Selector::Type::Compound:
    {
        auto c_sel = std::static_pointer_cast<Rule::CompoundSel>(sel);
        for (auto& s : c_sel->sels) {
            PrepareSel(s, rule_idx);
        }
    }
        break;
    default:
        assert(0);
    }
}

void EvalRule::TopologicalSorting::
AddRuleDepend(const RulePtr& rule, int rule_idx)
{
    for (int j = 0, m = m_rules.size(); j < m; ++j) {
        if (rule == m_rules[j]) {
            m_in_deg[rule_idx]++;
            m_out_ops[j].push_back(rule_idx);
            break;
        }
    }
}

}