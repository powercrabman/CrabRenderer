#pragma once

class PBRDemo : public Scene
{
public:
    void Init() override;

    void OnEnter() override;
    void OnExit() override;

    void OnUpdate(TimeStamp& in_ts) override;
    void OnRender(TimeStamp& in_ts) override;
    void OnRenderGUI(TimeStamp& in_ts) override;

    void OnEvent(CrabEvent& in_event) override;

    IMPLEMENT_SCENE(PBRDemo);

private:
    void _BuildPostProcess() const;

    using Vertex = Vertex3D_PosNormalTexTangent;

    //===================================================
    //                    Component
    //===================================================

    struct MeshComponent
    {

        eTopology         topology;
        Ref<Mesh>         mesh;
        Ref<VertexShader> vertexShader;
        Ref<PixelShader>  pixelShader;

        TextureList        textureList;
        SamplerStateList   samplerList;
        ConstantBufferList constantBufferList;
    };

    struct ModelComponent
    {
        eTopology         topology;
        Ref<Model>        model;
        Ref<VertexShader> vertexShader;
        Ref<PixelShader>  pixelShader;

        SamplerStateList   samplerList;
        ConstantBufferList constantBufferList;
    };

    struct CameraControllerComponent
    {
        CameraComponent camera;
        float           moveSpeed;
        float           rotateSpeed;
    };

    struct MaterialComponent
    {
        Vec3  albedo;
        float metallic;
        float roughness;
    };

    struct LightComponent
    {
        Vec3  radiance     = { 1.f, 1.f, 1.f };
        float fallOffStart = 3.f;
        float fallOffEnd   = 100.f;
        float spotPower    = 1.f;
    };

    //===================================================
    //                    cbuffer
    //===================================================

    cbuffer CB_Object
    {
        Mat4 world;
        Mat4 worldInvTranspose;
    };

    cbuffer CB_ObjectPixelShader
    {
        MaterialComponent material = {};
        Vec3              pad      = {};

        Bool  useHeightMap = false;
        float heightScale  = 0.015f;
        Bool  useNormalMap = false;
        Bool  useAlbedoMap = false;

        Bool useRoughnessMap = false;
        Bool useMetallicMap  = false;
        Bool useAOMap        = false;
    };

    cbuffer CB_Camera
    {
        Mat4 viewProj;
        Vec3 eyePosW;
    };

    cbuffer CB_LightData
    {
        Vec3  radiance;
        float fallOffStart;

        Vec3  position;
        float fallOffEnd;

        Vec3  direction;
        float spotPower;
    };

    cbuffer CB_Light
    {
        CB_LightData light;
    };

    cbuffer CB_HDRI
    {
        float exposure;
        float gamma;
    };

    cbuffer CB_Blur
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
    Ref<VertexShader> m_basicVS;
    Ref<PixelShader>  m_basicPS;

    // sampler
    Ref<SamplerState> m_wrapSampler;
    Ref<SamplerState> m_clampSampler;

    // rasterizer
    Ref<RasterizerState> m_rsState;
    bool                 m_enableWireframeMode = false;
    bool                 m_enableCulling       = true;
    bool                 m_rsStateDirty        = true;

    // constant buffer
    Ref<ConstantBuffer<CB_Object>>            m_cbObject;
    Ref<ConstantBuffer<CB_Camera>>            m_cbCamera;
    Ref<ConstantBuffer<CB_HDRI>>              m_cbHDRI;
    Ref<ConstantBuffer<CB_Blur>>              m_cbBlur;
    Ref<ConstantBuffer<CB_BlurCombine>>       m_cbBlurCombine;
    Ref<ConstantBuffer<CB_Light>>             m_cbLight;
    Ref<ConstantBuffer<CB_ObjectPixelShader>> m_cbObjectPixelShader;

    // entity
    Entity m_cameraEntity;
    Entity m_lightEntity;
    Entity m_sphereEntity;
    Entity m_modelEntity;

    // texture
    Ref<Texture2D> m_envMapTex;
    Ref<Texture2D> m_irradianceTex;
    Ref<Texture2D> m_specularTex;
    Ref<Texture2D> m_brdfLUT;

    // texture for PBR
    Ref<Texture2D> m_normalTex;
    Ref<Texture2D> m_albedoTex;
    Ref<Texture2D> m_metallicTex;
    Ref<Texture2D> m_roughnessTex;
    Ref<Texture2D> m_aoTex;
    Ref<Texture2D> m_heightTex;

    // hdri
    float m_hdriExposure = 1.f;
    float m_hdriGamma    = 2.2f;
    bool  m_hdriDirty    = true;

    // Post Process
    Ref<PostProcess> m_postProcess;

    // Blur
    float  m_blurStrength           = 0.3f;
    uint32 m_blurCount              = 3;
    bool   m_blurDirty              = true;
    bool   m_needPostProcessRebuild = true;

    // Entity
    bool  m_useHeightMap    = false;
    float m_heightScale     = 0.015f;
    bool  m_useNormalMap    = false;
    bool  m_useAlbedoMap    = false;
    bool  m_useRoughnessMap = false;
    bool  m_useMetallicMap  = false;
    bool  m_useAOMap        = false;
};
