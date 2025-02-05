#pragma once

/*!
 * 6. LightDemo.h
 * ===================================
 * very simple light demo
 * only implement point light
 * and use phong shading and blinn-phong shading
 *
 * next step: implement directional light and spot light
 * additional, implement load model load
 */

class LightDemo : public Scene
{
public:
    void Init() override;
    void OnEnter() override;
    void OnExit() override;

    void OnUpdate(TimeStamp& in_ts) override;
    void OnRender(TimeStamp& in_ts) override;
    void OnRenderGUI(TimeStamp& in_ts) override;

    void OnEvent(CrabEvent& in_event) override;

    IMPLEMENT_SCENE(LightDemo);

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

    // Constant Buffer

    cbuffer CB_Object
    {
        Mat4          world;
        Mat4          worldInvTranspose;
        PhongMaterial material;
        float         pad[3];
        Bool          useTexture;
    };

    cbuffer CB_Camera
    {
        Mat4 viewProj;
        Vec3 eyePos;
    };

    cbuffer CB_Light
    {
        Vec3  lightColor;
        float lightIntensity;

        Vec3  lightPos;
        float falloffStart;

        float falloffEnd;
        Vec3  pad;

        Bool useSmoothStep;
    };

    // entity
    Entity m_sphereEntity;
    Entity m_lightEntity;
    Entity m_cameraEntity;
    Entity m_floorEntity;
    bool   m_showNormal = false;

    // constant buffer
    Ref<D11_ConstantBuffer<CB_Object>> m_cbObject;
    Ref<D11_ConstantBuffer<CB_Camera>> m_cbCamera;
    Ref<D11_ConstantBuffer<CB_Light>>  m_cbLight;

    // shader
    Ref<D11_VertexShader> m_lightVS;
    Ref<D11_VertexShader> m_normalVS;

    enum eShaderMode
    {
        Phong,
        BlinnPhong
    };
    Ref<D11_PixelShader> m_phongPS;
    Ref<D11_PixelShader> m_blinnPhongPS;
    Ref<D11_PixelShader> m_normalPS;
    eShaderMode          m_shaderMode = eShaderMode::Phong;

    // rasterizer state
    Ref<D11_RasterizerState> m_rsState;
    bool                     m_wireframe  = false;
    bool                     m_cullenable = true;

    Ref<D11_DepthStencilState> m_dsState;

    float m_deltaPitch    = 15.f * DEG2RAD;
    float m_deltaYaw      = 45.f * DEG2RAD;
    float m_radius        = 5.f;
    bool  m_autoRotate    = true;
    bool  m_useSmoothStep = false;

    // Camera
    float m_moveSpeed = 3.f;
    float m_rotSpeed  = 0.1f;
};
