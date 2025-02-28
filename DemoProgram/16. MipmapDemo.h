#pragma once

class MipmapDemo : public Scene
{
public:
    void Init() override;

    void OnEnter() override;
    void OnExit() override;

    void OnUpdate(TimeStamp& in_ts) override;
    void OnRender(TimeStamp& in_ts) override;
    void OnRenderGUI(TimeStamp& in_ts) override;

    void OnEvent(CrabEvent& in_event) override;

    IMPLEMENT_SCENE(MipmapDemo);

private:
    void _DrawGUI();
    void _BuildPostProcessData();

    using Vertex = Vertex_Pos3D_Normal_Tex_Color;

    struct TessDemoVertex
    {
        Vec3 position;
    };

private:
    //===================================================
    //                  Components
    //===================================================

    struct CameraControllerComponent
    {
        CameraComponent camera;
        float           moveSpeed;
        float           rotSpeed;
    };

    struct MeshComponent
    {
        Ref<D11_Mesh> mesh = nullptr;

        eTopology topology = eTopology::TriangleList;

        Ref<D11_VertexShader>   vertexShader   = nullptr;
        Ref<D11_GeometryShader> geometryShader = nullptr;
        Ref<D11_PixelShader>    pixelShader    = nullptr;

        std::vector<Ref<D11_ConstantBufferBase>> constantBuffers = {};
        std::vector<Ref<D11_Texture>>            textures        = {};
        std::vector<Ref<D11_SamplerState>>       samplers        = {};
    };

    struct MaterialComponent
    {
        Vec4  ambient;
        Vec4  diffuse;
        Vec4  specular;
        float shininess;

        bool useTexture;
    };

    struct DrawNormalComponent
    {
        int dummy;
    };

    //===================================================
    //                 Constant Buffer
    //===================================================

    cbuffer CB_Object
    {
        Mat4 world;
        Mat4 worldInvTranspose;

        // material
        Vec4  ambient;
        Vec4  diffuse;
        Vec4  specular;
        float shininess;

        Bool useTexture;
    };

    cbuffer CB_Camera
    {
        Mat4 viewProj;
        Vec3 eyePos;
    };

    cbuffer CB_PixelShader
    {
        float diffuseMapStrength;
        float specularMapStrength;
        float speculerShininess;
        Bool  useSmoothStep;
    };

private:
    // textures
    Ref<D11_Texture> m_cubeTexture;
    Ref<D11_Texture> m_floorTexture;
    Ref<D11_Texture> m_diffuseCubeMap;
    Ref<D11_Texture> m_specularCubeMap;

    // entity
    Entity m_camera;

    // shaders
    Ref<D11_VertexShader> m_commonVS;
    Ref<D11_PixelShader>  m_cubemapPS;
    Ref<D11_PixelShader>  m_IBLPS;

    // constant buffer
    Ref<D11_ConstantBuffer<CB_Object>>       m_cbObject;
    Ref<D11_ConstantBuffer<CB_Camera>>       m_cbCamera;
    Ref<D11_ConstantBuffer<CB_PixelShader>>  m_cbPixelShader;
    CB_PixelShader                           m_cbPixelShaderData;
    bool                                     m_cbPixelShaderDirty = true;

    // gui flags
    bool m_rotateMode = false;

    // post processing
    std::vector<Ref<PostProcess>> m_ppStack;

    bool m_ppStackDirty         = true;
    bool m_enablePostProcessing = true;

    // anti aliasing values
    bool  m_enableAntiAliasing = false;
    int32 m_fxaaQuality        = 4;

    // render state
    Ref<D11_SamplerState>    m_commonSS;
    Ref<D11_RasterizerState> m_wireframeRS;
    Ref<D11_RasterizerState> m_solidRS;

    bool m_wireframeMode = false;

    // draw normal
    bool                    m_drawNormal = false;
    Ref<D11_VertexShader>   m_drawNormalVS;
    Ref<D11_PixelShader>    m_drawNormalPS;
    Ref<D11_GeometryShader> m_drawNormalGS;

    // bloom
    bool  m_enableBloom     = false;
    float m_bloomThreshold  = 0.8f;
    int32 m_blurStrength    = 10;
    float m_blurBlendFactor = 0.7f;

    Ref<PostProcess> m_thresholdPP;
    Ref<PostProcess> m_combinePP;
};
