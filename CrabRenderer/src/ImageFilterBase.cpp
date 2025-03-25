#include "InternalPCH.h"


#include "ImageFilterBase.h"

#include "GlobalState.h"
#include "RenderFactory.h"
#include "RenderStates.h"
#include "RenderTarget.h"
#include "Renderer.h"
#include "ShaderResource.h"
#include "Shaders.h"

namespace crab
{

ImageFilterBase::ImageFilterBase()
{
}

ImageFilterBase::~ImageFilterBase()
{
}

Ref<Texture2D> ImageFilterBase::GetOutputTexture() const
{
    return m_renderTarget->GetTexture();
}

void ImageFilterBase::Bind()
{
    auto& r = GetRenderer();
    r.ResetFrameBuffer();

    m_renderTarget->Bind(nullptr);
    m_vertexShader->Bind();
    m_pixelShader->Bind();
    r.ResetHullShader();
    r.ResetDomainShader();
    r.ResetGeometryShader();
    r.SetViewport(m_viewport);

    m_inputTexture->Bind(0, eShaderFlags_PixelShader);
    m_linearClampSampler->Bind(0, eShaderFlags_PixelShader);
}

void ImageFilterBase::Init_Internal(uint32                   in_width,
                                    uint32                   in_height,
                                    const Ref<VertexShader>& in_vs,
                                    const Ref<PixelShader>&  in_ps,
                                    const Ref<Texture2D>&    in_inputTexture)
{
    m_renderTarget = factory::CreateRenderTarget(
        in_width,
        in_height,
        eFormat::Float16_4,
        eBindFlags_ShaderResource | eBindFlags_RenderTarget,
        MSAA::DisableMSAA());

    m_viewport = Viewport {
        static_cast<float>(in_width),
        static_cast<float>(in_height),
    };
    m_vertexShader       = in_vs;
    m_pixelShader        = in_ps;
    m_inputTexture       = in_inputTexture;
    m_linearClampSampler = GlobalState::Sampler_Linear_Clamp();
}

}   // namespace crab