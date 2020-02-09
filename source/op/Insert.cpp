#include "ce/op/Insert.h"
#include "ce/EvalExpr.h"
#include "ce/Variant.h"
#include "ce/Geometry.h"

namespace ce
{
namespace op
{

void Insert::Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out,
                     const EvalContext& ctx)
{
    if (in.empty() || !in[0]) {
        out.resize(1, nullptr);
        return;
    }

    assert(in.size() == 1);
    auto geo = std::make_shared<Geometry>(*in[0]);
    geo->SetFilepath(m_geo_path);
    out.push_back(geo);
}

void Insert::Setup(const std::vector<cgac::ExprNodePtr>& parms,
                   const Rule::CompoundSel& selectors, const EvalContext& ctx)
{
    assert(parms.size() >= 1 && parms.size() <= 3);

    auto path_var = EvalExpr::Eval(parms[0], ctx);
    SetGeoPath(check_string(path_var));
}

}
}