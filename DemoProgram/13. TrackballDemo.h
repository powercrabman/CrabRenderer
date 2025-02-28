#pragma once
#pragma once

class TrackballDemo : public Scene
{
public:
    void Init() override;

    void OnEnter() override;
    void OnExit() override;

    void OnUpdate(TimeStamp& in_ts) override;
    void OnRender(TimeStamp& in_ts) override;
    void OnRenderGUI(TimeStamp& in_ts) override;

    void OnEvent(CrabEvent& in_event) override;

    IMPLEMENT_SCENE(TrackballDemo);

private:
    void _DrawGUI();
    void _BuildPostProcessData();

    // return value is intersection point in world space
    // if there is no intersection, return Vec3::Zero

    bool _MousePicking(uint32  in_x,
                       uint32  in_y,
                       Entity  in_cameraEntity,
                       Entity* out_selectedEntity,
                       Vec3*   out_intersectionPoint,
                       Ray*    out_ray);

    void _UpdateTrackball();

    using Vertex = Vertex_Pos3D_Normal_Tex_Color;

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
        eTopology             topology;
        Ref<D11_Mesh>         mesh;
        Ref<D11_VertexShader> vertexShader;
        Ref<D11_PixelShader>  pixelShader;

        std::vector<Ref<D11_ConstantBufferBase>> constantBuffers;
        D11_TextureArray                         textureArray;
        bool                                     useTexture;
    };

    struct MaterialComponent
    {
        Vec4  ambient;
        Vec4  diffuse;
        Vec4  specular;
        float shininess;
    };

    struct OBBComponent
    {
        Vec3 size;
        Vec3 center;   // local space

        // for graphic debug
        Ref<D11_Mesh> debugMesh;
    };

    struct BoundingShpereComponent
    {
        float radius;
        Vec3  center;   // local space

        // for graphic debug
        Ref<D11_Mesh> debugMesh;
    };

    //===================================================
    //                 Constant Buffer
    //===================================================

    cbuffer CB_Object
    {
        Mat4 world;
        Mat4 worldInvTranspose;
        Vec4 ambient;
        Vec4 diffuse;
        Vec4 specular;

        float  shininess;
        Bool   useTexture;
        uint32 entityID;
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
    Ref<D11_Texture>      m_cubeTexture;
    Ref<D11_Texture>      m_floorTexture;
    Ref<D11_Texture>      m_diffuseCubeMap;
    Ref<D11_Texture>      m_specularCubeMap;
    Ref<D11_SamplerState> m_sampler;

    // entity
    Entity m_camera;
    Entity m_debugSphere;

    // shaders
    Ref<D11_VertexShader> m_commonVS;
    Ref<D11_PixelShader>  m_cubemapPS;
    Ref<D11_PixelShader>  m_IBLPS;
    Ref<D11_PixelShader>  m_debugPS;

    // constant buffer
    Ref<D11_ConstantBuffer<CB_Object>>      m_cbObject;
    Ref<D11_ConstantBuffer<CB_Camera>>      m_cbCamera;
    Ref<D11_ConstantBuffer<CB_PixelShader>> m_cbPixelShader;
    CB_PixelShader                          m_cbPixelShaderData;
    bool                                    m_cbPixelShaderDirty = true;

    // gui flags
    bool m_rotateMode = false;

    // rasterizer state
    Ref<D11_RasterizerState> m_rsState;
    bool                     m_rsStateDirty = true;
    bool                     m_wireframe    = false;
    bool                     m_cullenable   = true;

    // post processing
    std::vector<Ref<PostProcess>> m_ppStack;

    bool m_ppStackDirty         = true;
    bool m_enablePostProcessing = true;

    // anti aliasing values
    bool  m_enableAntiAliasing = true;
    int32 m_fxaaQuality        = 4;

    // snapshot
    char m_snapshotFileName[256] = "snapshot.png";

    // mouse picking
    Entity m_selectedEntity     = Entity::s_null;
    bool   m_visibleBoundingBox = false;

    // depth stencil state
    Ref<D11_DepthStencilState> m_defaultDSS;
    Ref<D11_DepthStencilState> m_debugDDS;

    // for trackball
    Vec3 m_intersectionPoint = Vec3::Zero;
    Ray  m_ray;

    enum class eTrackballMode
    {
        None,
        Rotate,
        Translate
    };
    eTrackballMode m_trackballMode = eTrackballMode::None;
};
