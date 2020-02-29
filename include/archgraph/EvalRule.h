#pragma once

#include "archgraph/typedef.h"
#include "archgraph/Rule.h"
#include "archgraph/EvalContext.h"

#include <cga/typedef.h>

#include <map>
#include <memory>
#include <vector>
#include <sstream>

namespace archgraph
{

class EvalRule
{
public:
    EvalRule() {}

    void AddRule(const RulePtr& rule);

    RulePtr QueryRule(const std::string& name) const;

    void OnLoadFinished(const EvalContext& ctx);

    std::vector<GeoPtr> Eval(const std::vector<GeoPtr>& geos,
        const EvalContext& ctx);

    void SetFilepath(const std::string& filepath) { m_filepath = filepath; }

    void Clear();

    auto& GetConsole() const { return m_console; }

private:
    void DeduceOps(const EvalContext& ctx);
    void DeduceOps(const EvalContext& ctx, const Rule::SelPtr& sel);

    std::vector<GeoPtr> Eval(const std::vector<GeoPtr>& geos,
        const std::vector<Rule::OpPtr>& ops, const EvalContext& ctx);
    std::vector<GeoPtr> Eval(const std::vector<GeoPtr>& geos,
        const Rule::CompoundSel& sel, const EvalContext& ctx);

    void ResolveParmsExpr(Operation& op, const EvalContext& ctx) const;

private:
    class TopologicalSorting
    {
    public:
        TopologicalSorting(const std::map<std::string, RulePtr>& rules);

        std::vector<RulePtr> Sort();

    private:
        void PrepareOp(const Rule::OpPtr& op, int rule_idx);
        void PrepareSel(const Rule::SelPtr& sel, int rule_idx);
        void AddRuleDepend(const RulePtr& rule, int rule_idx);

    private:
        std::vector<RulePtr> m_rules;

        std::vector<int>              m_in_deg;
        std::vector<std::vector<int>> m_out_ops;

    }; // TopologicalSorting

private:
    std::string m_filepath;

    std::map<std::string, RulePtr> m_rules;

    mutable std::vector<RulePtr> m_rules_sorted;

    mutable std::stringstream m_console;

}; // EvalRule

}