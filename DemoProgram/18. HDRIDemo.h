#pragma once

class HDRIDemo : public Scene
{
public:
    void Init() override;

    void OnEnter() override;
    void OnExit() override;

    void OnUpdate(TimeStamp& in_ts) override;
    void OnRender(TimeStamp& in_ts) override;
    void OnRenderGUI(TimeStamp& in_ts) override;

    void OnEvent(CrabEvent& in_event) override;

    IMPLEMENT_SCENE(HDRIDemo);

private:
    void _BuildPostProcess();

    struct Vertex
    {
        Vec3 position;
        Vec2 texCoord;
        Vec3 normal;
        Vec3 tangent;
    };

    //===================================================
    //                    Component
    //===================================================

    struct MeshComponent
    {
        eTopology             topology;
        Ref<D11_Mesh>         mesh;
        Ref<D11_VertexShader> vertexShader;
        Ref<D11_PixelShader>  pixelShader;

        D11_TextureList        texturelist;
        D11_SamplerStateList   samplerlist;
        D11_ConstantBufferList constantBufferList;
    };

    struct CameraControllerComponent
    {
        CameraComponent camera;
        float           moveSpeed;
        float           rotateSpeed;
    };

    //===================================================
    //                    cbuffer
    //===================================================

    cbuffer CB_Object
    {
        Mat4 world;
        Mat4 worldInvTranspose;

        bool useTexture;
    };

    cbuffer CB_Camera
    {
        Mat4 viewProj;
        Vec3 eyePosW;
    };

    cbuffer CB_PixelShader
    {
        Vec3 lightPosW;
        bool pad;
        Bool useDiffuseTexture;
        Bool useNormalTexture;
    };

    cbuffer CB_HDRI
    {
        float exposure;
        float gamma;
    };

    cbuffer CB_BlurSample
    {
        float dx;
        float dy;
    };

    cbuffer CB_BlurCombine
    {
        float blurStrength;
    };

private:
    // Shaders
    Ref<D11_VertexShader> m_commonVS;
    Ref<D11_PixelShader>  m_commonPS;
    Ref<D11_VertexShader> m_whiteVS;
    Ref<D11_PixelShader>  m_whitePS;
    Ref<D11_PixelShader>  m_hdriPS;

    // sampler
    Ref<D11_SamplerState> m_commonSampler;

    // rasterizer
    Ref<D11_RasterizerState> m_rsState;
    bool                     m_enableWireframeMode = false;
    bool                     m_enableCulling       = true;
    bool                     m_rsStateDirty        = true;

    // constant buffer
    Ref<D11_ConstantBuffer<CB_Object>>      m_cbObject;
    Ref<D11_ConstantBuffer<CB_Camera>>      m_cbCamera;
    Ref<D11_ConstantBuffer<CB_PixelShader>> m_cbLight;
    Ref<D11_ConstantBuffer<CB_HDRI>>        m_cbHDRI;
    Ref<D11_ConstantBuffer<CB_BlurSample>>  m_cbBlurSample;
    Ref<D11_ConstantBuffer<CB_BlurCombine>> m_cbBlurCombine;

    // cb pixel shader
    CB_PixelShader m_cbPixelShader;

    // entity
    Entity m_cameraEntity;
    Entity m_lightEntity;

    // texture
    Ref<D11_Texture2D> m_diffuseBlock;
    Ref<D11_Texture2D> m_normalBlock;
    Ref<D11_Texture2D> m_hdriTexture;
    Ref<D11_Texture2D> m_skyboxTexture;

    // hdri
    float m_hdriExposure;
    float m_hdriGamma;
    float m_hdriDirty;

    // Post Process
    Ref<PostProcess> m_postProcess;

    // Blur
    float m_blurStrength = 0.15f;
    bool  m_blurDirty    = true;
};
