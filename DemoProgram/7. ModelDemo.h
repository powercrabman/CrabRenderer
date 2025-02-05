#pragma once

class ModelDemo : public Scene
{
public:
    void Init() override;
    void OnEnter() override;
    void OnExit() override;

    void OnUpdate(TimeStamp& in_ts) override;
    void OnRender(TimeStamp& in_ts) override;
    void OnRenderGUI(TimeStamp& in_ts) override;

    void OnEvent(CrabEvent& in_event) override;

    IMPLEMENT_SCENE(ModelDemo);

private:
    void _CreateRasterizerState();

    using Vertex = Vertex_Pos3D_Normal_Tex_Color;

    // Component + Resource
    struct PhongMaterial
    {
        Vec4  ambient;
        Vec4  diffuse;
        Vec4  specular;
        float shininess;

        TextureArray textureArray;
    };

    struct MeshRenderer
    {
        eTopology             topology = eTopology::TriangleList;
        Ref<D11_Mesh>         mesh;
        Ref<D11_VertexShader> vertexShader;
        Ref<D11_PixelShader>  pixelShader;
    };

    struct ModelRenderer
    {
        eTopology             topology = eTopology::TriangleList;
        Ref<D11_Model>        model;
        Ref<D11_VertexShader> vertexShader;
        Ref<D11_PixelShader>  pixelShader;
    };

    struct MaterialComponent
    {
        PhongMaterial material;
        bool          useTexture;
    };

    struct NormalRenderer
    {
        Ref<D11_Mesh>         mesh;
        Ref<D11_VertexShader> vertexShader;
        Ref<D11_PixelShader>  pixelShader;
    };

    struct LightComponent
    {
        Vec3       lightColor;
        float      lightIntensity;
        float      falloffStart;   // Point + Spot
        float      falloffEnd;     // Point + Spot
        float      spotPower;      // Spot
        eLightType lightType;
    };

    cbuffer CB_Material
    {
        Vec4  ambient;
        Vec4  diffuse;
        Vec4  specular;
        float shininess;
    };

    cbuffer CB_Object
    {
        Mat4        world;
        Mat4        worldInvTranspose;
        CB_Material material;
        Bool        useTexture;
    };

    cbuffer CB_Camera
    {
        Mat4 viewProj;
        Vec3 eyePos;
    };

    cbuffer CB_LightComponent
    {
        // 16
        Vec3  lightColor;
        float lightIntensity;

        // 16
        Vec3  direction;
        float falloffStart;

        // 16
        Vec3  position;
        float falloffEnd;

        // 16
        float spotPower;
        float pad[3];
    };

    cbuffer CB_PixelShader
    {
        CB_LightComponent dirLight;
        CB_LightComponent pointLight;
        CB_LightComponent spotLight;
        Bool              useSmoothStep;
    };

    // entity
    Entity m_modelEntity;
    Entity m_cameraEntity;

    Entity m_pointLightEntity;
    Entity m_dirLightEntity;
    Entity m_spotLightEntity;
    bool   m_showNormal = false;

    // constant buffer
    Ref<D11_ConstantBuffer<CB_Object>>      m_cbObject;
    Ref<D11_ConstantBuffer<CB_Camera>>      m_cbCamera;
    Ref<D11_ConstantBuffer<CB_PixelShader>> m_cbPixelShader;

    // shader
    Ref<D11_PixelShader>  m_normalPS;
    Ref<D11_VertexShader> m_normalVS;

    Ref<D11_VertexShader> m_commonVS;
    Ref<D11_PixelShader>  m_blinnPhongPS;

    // rasterizer state
    Ref<D11_RasterizerState> m_rsState;
    bool                     m_wireframe  = false;
    bool                     m_cullenable = true;

    // depth stencil state
    Ref<D11_DepthStencilState> m_dsState;

    bool m_useSmoothStep = false;

    // Camera
    float m_moveSpeed = 3.f;
    float m_rotSpeed  = 0.1f;
};

/*
todo : direction light + point light + spot light
next : cube map
next : rim light
next : post processing
*/