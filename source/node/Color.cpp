#include "cga/node/Color.h"
#include "cga/Geometry.h"

#include <polymesh3/Polytope.h>

namespace cga
{
namespace node
{

void Color::Execute(const std::vector<GeoPtr>& in, std::vector<GeoPtr>& out)
{
    assert(in.size() == 1);
    auto geo = std::make_shared<Geometry>(*in[0]);
    geo->SetColor(m_color);
    out.push_back(geo);
}

}
}