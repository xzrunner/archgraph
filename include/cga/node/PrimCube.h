#pragma once

#include "cga/Node.h"

#include <SM_Vector.h>

namespace cga
{
namespace node
{

class PrimCube : public Node
{
public:
    PrimCube()
    {
        m_exports = {
            { "out" },
        };
    }

    virtual void Execute() override;

    void SetSize(const sm::vec3& size);

private:
    void BuildModel();

private:
    sm::vec3 m_size = sm::vec3(1, 1, 1);

    RTTR_ENABLE(Node)

}; // PrimCube

}
}