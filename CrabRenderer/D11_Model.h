#pragma once

#include "D11_Mesh.h"
#include "VertexType.h"

struct aiNode;
struct aiScene;
struct aiMesh;

namespace crab
{

struct D11_Mesh;
struct D11_Model;
struct Texture;

//===================================================
// Model
//===================================================

struct D11_Model
{
    std::vector<Ref<D11_Mesh>> meshes;
    std::vector<Ref<Texture>>  textures;
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
    std::vector<Ref<Texture>>     GetTextures();

private:
    void _ProcessNode(aiNode* in_node, const aiScene* in_scene);

    MeshData<Vertex> _ProcessMesh(aiMesh* in_mesh, const aiScene* in_scene);
    Ref<Texture>     _ProcessMaterial(aiMesh* in_mesh, const aiScene* in_scene);

    std::vector<MeshData<Vertex>> m_meshDatas;
    std::vector<Ref<Texture>>     m_textures;
    std::filesystem::path         m_directory;
};

}   // namespace crab
