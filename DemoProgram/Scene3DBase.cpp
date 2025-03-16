#include "pch.h"

#include "Scene3DBase.h"

Scene3DBase::~Scene3DBase()
{
}

void Scene3DBase::Init()
{
    auto&       r  = GetRenderer();
    const auto& sc = r.GetSwapChain();

    if (!sc->IsHDR())
    {
        CRAB_DEBUG_BREAK("this class requires HDR swap chain.");
        return;
    }

    //===================================================
    // Core Renderer Resource
    //===================================================

    m_backBufferHDR         = sc->GetBackBufferHDR();
    m_backBufferDepthBuffer = sc->GetDepthBuffer();

    auto [screenWidth, screenHeight] = GetAppWindow().GetResolution();
    m_screenViewport                 = Viewport { 0.f, 0.f, static_cast<float>(screenWidth), static_cast<float>(screenHeight) };
    _CreateStagingTexture(screenWidth, screenHeight);

    // camera
    m_cameraEntity = CreateEntity("Camera");
    m_cameraEntity.CreateComponent<CameraComponent>(
        CameraComponent {
            .projectionType = eProjectionType::Perspective,
            .nearZ          = 0.1f,
            .farZ           = 1000.f,
            .fov            = 45.f * DEG2RAD,
            .aspect         = GetAppWindow().GetAspect() });
}

void Scene3DBase::OnEnter()
{
}

void Scene3DBase::OnExit()
{
}

void Scene3DBase::OnUpdate(TimeStamp& in_ts)
{
    if (Input::IsKeyPressed(eKey::Escape))
        GetApplication().Quit();
}

void Scene3DBase::OnRender(TimeStamp& in_ts)
{
    auto [screenWidth, screenHeight] = GetAppWindow().GetResolution();
    auto&       r                    = GetRenderer();
    const auto* pipe                 = GetGlobalPipeline();

    //===================================================
    // Update LightData
    //===================================================

    CrabPass::LightShadowPass<"PBR">(this, GetCameraEntity());

    //===================================================
    // Begin Render
    //===================================================

    {
        m_backBufferHDR->Bind(m_backBufferDepthBuffer);
        m_backBufferHDR->Clear(color4::BLACK);
        m_backBufferDepthBuffer->Clear(true, 1.f, false, 0);
        r.SetViewport(0, 0, screenWidth, screenHeight);

        // Update Camera
        auto& cmr = GetCameraComponent();

        GetGlobalConstants()->UpdateCamera(
            CameraConstant {
                .view           = cmr.GetView(GetCameraTransform()),
                .viewProj       = cmr.GetViewProj(GetCameraTransform()),
                .invViewProj    = cmr.GetViewProj(GetCameraTransform()).Invert(),
                .cameraPosition = GetCameraTransform().position,
            });
    }

    //===================================================
    // Main Render
    //===================================================

    CrabPass::SkyboxPass<"Skybox">(this);
    CrabPass::PBRPass<"PBR">(this, {}, IsWireframeModeEnabled() ? pipe->GetPBRWireframe() : nullptr);

    if (IsDrawNormalEnabled())
    {
        CrabPass::DrawNormalPass<"PBR">(this);
    }

    //===================================================
    // Mirror Reflection
    //===================================================

    CrabPass::MirrorPass<"Mirror">(
        this,
        m_backBufferDepthBuffer,
        GetCameraEntity(),
        1,
        [&](Scene* in_scene)
        {
            CrabPass::SkyboxPass<"Skybox">(this, PipelineBindArgument { 1 }, pipe->GetSkyboxReflection());
            CrabPass::PBRPass<"PBR">(this, PipelineBindArgument { 1 }, IsWireframeModeEnabled() ? pipe->GetPBRRefractionWireframe() : pipe->GetPBRReflection());
            CrabPass::PBRPass<"Mirror">(this, PipelineBindArgument { 1 }, IsWireframeModeEnabled() ? pipe->GetPBRWireframeOnMask() : pipe->GetPBROnMask());
        });
}

void Scene3DBase::OnPostRender(TimeStamp& in_ts)
{
    const auto& swapChain = GetRenderer().GetSwapChain();
    if (m_postProcessProp.dirtyBit)
    {
        // build post process
        m_postProcess.ClearFilterList();
        auto [screenWidth, screenHeight] = GetAppWindow().GetResolution();

        // sampling
        m_postProcess.AddFilter(
            ImageFilterFactory::CreateSampling(
                screenWidth, screenHeight, m_stagingBackBufferTexture));

        // bloom
        if (m_postProcessProp.useBloom)
        {
            for (uint32 i = 0; i < m_postProcessProp.bloomBlurCount; ++i)
            {
                uint32 width  = screenWidth >> (i + 1);
                uint32 height = screenHeight >> (i + 1);
                m_postProcess.AddFilter(
                    ImageFilterFactory::CreateBlurDown(
                        width, height, m_postProcess.GetLastFilter()->GetOutputTexture()));
            }
            for (uint32 i = 0; i < m_postProcessProp.bloomBlurCount; ++i)
            {
                uint32 width  = screenWidth >> (m_postProcessProp.bloomBlurCount - i);
                uint32 height = screenHeight >> (m_postProcessProp.bloomBlurCount - i);
                m_postProcess.AddFilter(
                    ImageFilterFactory::CreateBlurUp(
                        width,
                        height,
                        m_postProcess.GetLastFilter()->GetOutputTexture(),
                        BlurUpConstant { m_postProcessProp.bloomBlurRadius }));
            }

            m_postProcess.AddFilter(
                ImageFilterFactory::CreateCombine(
                    screenWidth,
                    screenHeight,
                    m_postProcess.GetLastFilter()->GetOutputTexture(),
                    m_stagingBackBufferTexture,
                    CombineConstant { m_postProcessProp.bloomCombineFactor },
                    &m_postprocessCombineConst));
        }

        // tone map
        if (m_postProcessProp.useToneMapping)
        {
            m_postProcess.AddFilter(
                ImageFilterFactory::CreateToneMapping(
                    screenWidth,
                    screenHeight,
                    m_postProcess.GetLastFilter()->GetOutputTexture(),
                    ToneMappingConstant {
                        .exposure = m_postProcessProp.exposure,
                        .gamma    = m_postProcessProp.gamma },
                    &m_postprocessToneMappingConst));
        }

        m_postProcess.GetLastFilter()->SetRenderTarget(swapChain->GetBackBuffer());
        m_postProcessProp.dirtyBit = false;
    }

    ID3D11Texture2D* stagingTexture    = m_stagingBackBufferTexture->GetResource<ID3D11Texture2D>();
    ID3D11Texture2D* backBufferTexture = swapChain->GetBackBufferHDR()->GetTexture()->GetResource<ID3D11Texture2D>();

    if (swapChain->UseMSAA())
    {
        ID3D11Texture2DUtil::ResolveTexture2D(
            backBufferTexture,
            stagingTexture);
    }
    else
    {
        ID3D11Texture2DUtil::CopyBetween(
            backBufferTexture,
            stagingTexture);
    }

    m_postProcess.Render();
}

void Scene3DBase::OnEvent(CrabEvent& in_event)
{
}

void Scene3DBase::EnablePostProcessBloom(bool in_enable)
{
    m_postProcessProp.useBloom = in_enable;
    m_postProcessProp.dirtyBit = true;
}

void Scene3DBase::SetBloomCombineFactor(float in_factor)
{
    CRAB_ASSERT(m_postprocessCombineConst != nullptr, "Constant buffer is not initialized.");
    m_postProcessProp.bloomCombineFactor = in_factor;
    m_postprocessCombineConst->WriteToBuffer(
        CombineConstant {
            .combineStrength = m_postProcessProp.bloomCombineFactor });
}

void Scene3DBase::SetBloomBlurRadius(float in_radius)
{
    m_postProcessProp.bloomBlurRadius = in_radius;
    m_postProcessProp.dirtyBit        = true;
}

void Scene3DBase::_CreateStagingTexture(uint32 in_width, uint32 in_height)
{
    const auto& sc             = GetRenderer().GetSwapChain();
    m_stagingBackBufferTexture = CreateRef<Texture2D>();
    m_stagingBackBufferTexture->Init(
        ID3D11Texture2DUtil::CreateTexture2D(
            in_width,
            in_height,
            m_backBufferHDR->GetFormat(),
            D3D11_USAGE_DEFAULT,
            eBindFlags_ShaderResource | eBindFlags_RenderTarget,
            eCPUAccessFlags_None)
            .Get());
}

void Scene3DBase::SetBloomBlurCount(uint32 in_count)
{
    m_postProcessProp.bloomBlurCount = in_count;
    m_postProcessProp.dirtyBit       = true;
}
