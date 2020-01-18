#include "cga/BuildInFuncs.h"
#include "cga/Variant.h"
#include "cga/FuncRegister.h"

#include <assert.h>

namespace cga
{
namespace func
{

VarPtr Print::Eval(const std::vector<VarPtr>& parms,
                   const std::vector<GeoPtr>& geos, std::ostream& console) const
{
    for (auto& p : parms)
    {
        if (p) {
            PrintVar(p, geos, console);
        }
        console << "\n";
    }
    return nullptr;
}

void Print::PrintVar(const VarPtr& var, const std::vector<GeoPtr>& geos,
                     std::ostream& console)
{
    if (!var) {
        return;
    }

    switch (var->Type())
    {
    case VarType::Boolean:
        console << std::static_pointer_cast<BoolVar>(var)->GetValue();
        break;
    case VarType::Float:
        console << std::static_pointer_cast<FloatVar>(var)->GetValue();
        break;
    case VarType::String:
    {
        auto str = std::static_pointer_cast<StringVar>(var)->GetValue();
        auto itr_func = FuncRegister::Instance()->QueryAttrFunc(str);
        if (itr_func)
        {
            auto _var = itr_func->Eval(std::vector<VarPtr>(), geos, console);
            PrintVar(_var, geos, console);
        }
        else
        {
            console << str;
        }
    }
    break;
    default:
        assert(0);
    }
}

}
}