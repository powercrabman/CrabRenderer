#pragma once

#include "D11_Mesh.h"
#include "VertexType.h"

struct aiNode;
struct aiScene;
struct aiMesh;

namespace crab
{

class D11_Mesh;
class D11_Model;
class D11_Texture2D;

//===================================================
// Model
//===================================================

class D11_Model
{
public:
    static Ref<D11_Model> Create(
        const std::vector<Ref<D11_Mesh>>&    in_meshes,
        const std::vector<Ref<D11_Texture2D>>& in_textures);

    void Draw(bool in_useTexture);

    // temp -> private members
    std::vector<Ref<D11_Mesh>>    m_meshes;
    std::vector<Ref<D11_Texture2D>> m_textures;
};

//===================================================
// Model Loader
//===================================================

class D11_ModelLoader
{
public:
    using Vertex = Vertex_Pos3D_Normal_Tex_Color;

    bool Load(const std::filesystem::path& in_path);

    Ref<D11_Model>                CreateModel();
    std::vector<MeshData<Vertex>> GetMeshDatas();
    std::vector<Ref<D11_Texture2D>> GetTextures();

private:
    void _ProcessNode(aiNode* in_node, const aiScene* in_scene);

    MeshData<Vertex> _ProcessMesh(aiMesh* in_mesh, const aiScene* in_scene);
    Ref<D11_Texture2D> _ProcessMaterial(aiMesh* in_mesh, const aiScene* in_scene);

    std::vector<MeshData<Vertex>> m_meshDatas;
    std::vector<Ref<D11_Texture2D>> m_textures;
    std::filesystem::path         m_directory;
};

}   // namespace crab
