#include "CrabPch.h"

#include "D11_Mesh.h"

#include "D11_Buffers.h"
#include "D11_Model.h"
#include "D11_Renderer.h"

namespace crab
{

Ref<D11_Mesh> D11_Mesh::Create(const Ref<D11_VertexBuffer>& in_vb,
                               const Ref<D11_IndexBuffer>&  in_ib)
{
    auto mesh            = CreateRef<D11_Mesh>();
    mesh->m_vertexBuffer = in_vb;
    mesh->m_indexBuffer  = in_ib;
    return mesh;
}

void D11_Mesh::Draw() const
{
    auto* dx = D11_API;
    m_vertexBuffer->Bind();
    m_indexBuffer->Bind();
    dx->DrawIndexed(m_indexBuffer->GetIndexCount());
}

}   // namespace crab