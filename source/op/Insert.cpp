#include "cga/op/Insert.h"
#include "cga/EvalExpr.h"
#include "cga/Variant.h"
#include "cga/Geometry.h"

namespace cga
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

    auto path_var = EvalExpr::Eval(parms[0]);
    assert(path_var && path_var->Type() == VarType::String);
    SetGeoPath(path_var->ToString());
}

}
}