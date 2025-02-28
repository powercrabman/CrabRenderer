#pragma once

class PostProcessingDemo : public Scene
{
public:
    void Init() override;
    void OnEnter() override;
    void OnExit() override;

    void OnUpdate(TimeStamp& in_ts) override;
    void OnRender(TimeStamp& in_ts) override;
    void OnRenderGUI(TimeStamp& in_ts) override;

    void OnEvent(CrabEvent& in_event) override;

    IMPLEMENT_SCENE(PostProcessingDemo);

private:
    void _CreateRasterizerState();

    using Vertex = Vertex_Pos3D_Normal_Tex_Color;

    struct MeshRenderer
    {
        eTopology             topology = eTopology::TriangleList;
        Ref<D11_Mesh>         mesh;
        Ref<D11_VertexShader> vertexShader;
        Ref<D11_PixelShader>  pixelShader;
    };

    struct PhongMaterial
    {
        Vec4  ambient   = color::BLACK;
        Vec4  diffuse   = color::BLACK;
        Vec4  specular  = color::BLACK;
        float shininess = 1.f;

        D11_TextureArray textureArray;
        uint32           textureStartSlot = 0;
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

    cbuffer CB_Material
    {
        Vec4  ambient;
        Vec4  diffuse;
        Vec4  specular;
        float shininess;
        float pad[3];
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

    cbuffer CB_PixelShader
    {
        float diffuseMapStrength;
        float specularMapStrength;
        float speculerShininess;
        Bool  useSmoothStep;
    };

    float m_diffuseMapStrength  = 0.5f;
    float m_specularMapStrength = 0.5f;
    float m_specularShininess   = 1.f;
    bool  m_useSmoothStep       = true;
    bool  m_showNormal          = false;

    // entity
    Entity m_modelEntity;
    Entity m_cameraEntity;

    // cube texture
    Ref<D11_Texture> m_cubemapSpecular;
    Ref<D11_Texture> m_cubemapDiffuse;
    D11_TextureArray m_cubemapTextureArray;

    // constant buffer
    Ref<D11_ConstantBuffer<CB_Object>>      m_cbObject;
    Ref<D11_ConstantBuffer<CB_Camera>>      m_cbCamera;
    Ref<D11_ConstantBuffer<CB_PixelShader>> m_cbPixelShader;

    // shader
    Ref<D11_PixelShader>  m_normalPS;
    Ref<D11_VertexShader> m_normalVS;

    Ref<D11_VertexShader> m_commonVS;
    Ref<D11_PixelShader>  m_IBLPS;

    // rasterizer state
    Ref<D11_RasterizerState> m_rsState;
    bool                     m_wireframe  = false;
    bool                     m_cullenable = true;

    // depth stencil state
    Ref<D11_DepthStencilState> m_dsState;

    // Camera
    float m_moveSpeed = 3.f;
    float m_rotSpeed  = 0.1f;

    // PostProcessing
    std::vector<Ref<PostProcess>> m_postProcessStack;
    float                         m_strength        = 0.5f;
    float                         m_threshold       = 0.7f;
    bool                          m_ppConstantDirty = true;

    bool m_eneblePostProcessing = true;
};
