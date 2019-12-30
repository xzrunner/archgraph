#pragma once

#include "cga/typedef.h"
#include "cga/Rule.h"
#include "cga/EvalContext.h"

#include <cgac/typedef.h>

#include <map>
#include <memory>
#include <vector>

namespace cga
{

class EvalRule
{
public:
    EvalRule() {}

    void AddRule(const RulePtr& rule);
    void AddSymbol(const std::string& name, const cgac::ExprNodePtr& val);

    void OnLoadFinished();

    std::vector<GeoPtr> Eval(const std::vector<GeoPtr>& geos) const;

private:
    void DeduceOps();
    void TopologicalSorting() const;

    std::vector<GeoPtr> Eval(const std::vector<GeoPtr>& geos, const std::vector<Rule::OpPtr>& ops) const;

    void ResolveParmsExpr(Node& node) const;

private:
    std::map<std::string, RulePtr> m_rules;

    mutable std::vector<RulePtr> m_rules_sorted;

    EvalContext m_ctx;

}; // EvalRule

}