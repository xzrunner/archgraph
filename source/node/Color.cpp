#include "cga/node/Color.h"
#include "cga/Geometry.h"
#include "cga/NodeHelper.h"

#include <polymesh3/Polytope.h>

namespace cga
{
namespace node
{

void Color::Execute()
{
    m_geo.reset();

    auto prev_geo = NodeHelper::GetInputGeo(*this, 0);
    if (!prev_geo) {
        return;
    }

    m_geo = std::make_shared<Geometry>(*prev_geo);
    m_geo->SetColor(m_color);
}

}
}