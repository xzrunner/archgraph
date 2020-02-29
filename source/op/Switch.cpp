#include "archgraph/op/Switch.h"
#include "archgraph/EvalExpr.h"
#include "archgraph/Variant.h"
#include "archgraph/Geometry.h"
#include "archgraph/ArchGraph.h"

#include <cga/Parser.h>
#include <cga/Expression.h>

namespace archgraph
{
namespace op
{

void Switch::OnParmChanged(const std::string& parm_name)
{
    if (parm_name == ParmNames[static_cast<int>(Parm::CaseExprStrings)])
    {
        m_case_exprs.clear();
        m_case_exprs.reserve(m_case_expr_strs.size());
        for (auto& str : m_case_expr_strs)
        {
            cga::Parser parser(str.c_str(), ArchGraph::Instance()->GetStringPool());
            auto expr = cga::ExpressionParser::ParseExpression(parser);
            m_case_exprs.push_back(expr);
        }

        SetupExports();
    }
}

void Switch::Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
                     const EvalContext& ctx)
{
    assert(m_case_exprs.size() == m_exports.size() - 1);
    out.resize(m_exports.size(), nullptr);

    std::vector<std::vector<GeoPtr>> geos;
    geos.resize(out.size());
    for (auto& geo : in)
    {
        if (!geo) {
            continue;
        }

        size_t idx = out.size() - 1;
        for (size_t i = 0, n = m_case_exprs.size(); i < n; ++i)
        {
            auto var = EvalExpr::Eval(m_case_exprs[i], ctx, geo);
            if (check_bool(var)) {
                idx = i;
                break;
            }
        }

        assert(idx >= 0 && idx < out.size());
        geos[idx].push_back(geo);
    }
    for (size_t i = 0, n = out.size(); i < n; ++i)
    {
        if (geos[i].empty()) {
            continue;
        } else if (geos[i].size() == 1) {
            out[i] = std::make_shared<Geometry>(*geos[i].front());
        } else {
            out[i] = std::make_shared<Geometry>(geos[i]);
        }
    }
}

void Switch::Setup(const std::vector<cga::ExprNodePtr>& parms,
                   const Rule::CompoundSel& selectors, const EvalContext& ctx)
{
    assert(selectors.sels.size() >= 2
        && selectors.sels.back()->GetType() == Rule::Selector::Type::Single
        && !std::static_pointer_cast<Rule::SingleSel>(selectors.sels.back())->head);

    m_case_exprs.clear();
    m_case_exprs.reserve(selectors.sels.size());
    for (size_t i = 0, n = selectors.sels.size() - 1; i < n; ++i)
    {
        auto& sel = selectors.sels[i];
        assert(sel->GetType() == Rule::Selector::Type::Single);
        auto single = std::static_pointer_cast<Rule::SingleSel>(sel);
        assert(single->head);
        m_case_exprs.push_back(single->head);
    }

    SetupExports();
}

void Switch::SetupExports()
{
    m_exports.clear();
    m_exports.reserve(m_case_exprs.size());

    for (size_t i = 0, n = m_case_exprs.size(); i < n; ++i) {
        m_exports.push_back({ { OpVarType::Any, "case" + std::to_string(i) } });
    }
    m_exports.push_back({ { OpVarType::Any, "else" } });

    SetPortChanged(true);
}

}
}