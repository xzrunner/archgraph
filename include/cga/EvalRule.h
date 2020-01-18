#pragma once

#include "cga/typedef.h"
#include "cga/Rule.h"
#include "cga/EvalContext.h"

#include <cgac/typedef.h>

#include <map>
#include <memory>
#include <vector>
#include <sstream>

namespace cga
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
    std::string m_filepath;

    std::map<std::string, RulePtr> m_rules;

    mutable std::vector<RulePtr> m_rules_sorted;

    mutable std::stringstream m_console;

}; // EvalRule

}