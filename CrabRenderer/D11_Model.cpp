#include "CrabPch.h"

#include "D11_Mesh.h"
#include "D11_Model.h"
#include "D11_RenderState.h"
#include "Texture.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace crab
{

//===================================================
// Model Loader
//===================================================

bool D11_ModelLoader::Load(const std::filesystem::path& in_path)
{
    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(in_path.string(),
                                              aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

    if (!pScene)
    {
        CRAB_DEBUG_BREAK_V("Failed to load mesh file: {0}", in_path.string().c_str());
        return false;
    }

    m_meshDatas.clear();
    m_textures.clear();
    m_directory = in_path.parent_path();
    _ProcessNode(pScene->mRootNode, pScene);
    return true;
}

Ref<D11_Model> D11_ModelLoader::CreateModel()
{
    std::vector<Ref<D11_Mesh>> meshes;
    for (auto& meshData: m_meshDatas)
    {
        meshes.push_back(D11_Mesh::Create(meshData));
    }

    Ref<D11_Model> model = CreateRef<D11_Model>();
    model->meshes        = meshes;
    model->textures      = m_textures;
    return model;
}

std::vector<MeshData<D11_ModelLoader::Vertex>> D11_ModelLoader::GetMeshDatas()
{
    return m_meshDatas;
}

std::vector<Ref<Texture>> D11_ModelLoader::GetTextures()
{
    return m_textures;
}

void D11_ModelLoader::_ProcessNode(aiNode* in_node, const aiScene* in_scene)
{
    for (UINT i = 0; i < in_node->mNumMeshes; i++)
    {
        aiMesh* mesh = in_scene->mMeshes[in_node->mMeshes[i]];
        m_meshDatas.push_back(_ProcessMesh(mesh, in_scene));
        m_textures.push_back(_ProcessMaterial(mesh, in_scene));
    }
    for (UINT i = 0; i < in_node->mNumChildren; i++)
    {
        _ProcessNode(in_node->mChildren[i], in_scene);
    }
}

MeshData<D11_ModelLoader::Vertex> D11_ModelLoader::_ProcessMesh(aiMesh* in_mesh, const aiScene* in_scene)
{
    std::vector<Vertex> vertices;
    std::vector<UINT>   indices;

    // Process vertices
    for (UINT i = 0; i < in_mesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.position.x = in_mesh->mVertices[i].x;
        vertex.position.y = in_mesh->mVertices[i].y;
        vertex.position.z = in_mesh->mVertices[i].z;
        if (in_mesh->mTextureCoords[0])
        {
            vertex.texCoord.x = (float)in_mesh->mTextureCoords[0][i].x;
            vertex.texCoord.y = (float)in_mesh->mTextureCoords[0][i].y;
        }
        else
        {
            vertex.texCoord = { 0.0f, 0.0f };
        }

        if (in_mesh->HasNormals())
        {
            vertex.normal.x = in_mesh->mNormals[i].x;
            vertex.normal.y = in_mesh->mNormals[i].y;
            vertex.normal.z = in_mesh->mNormals[i].z;
        }
        else
        {
            vertex.normal = { 0.0f, 0.0f, 0.0f };
        }

        if (in_mesh->mColors[0])
        {
            aiColor4D aiColor = in_mesh->mColors[0][i];
            vertex.color      = Color(aiColor.r, aiColor.g, aiColor.b, aiColor.a);
        }
        else
        {
            vertex.color = Color(1.0f, 1.0f, 1.0f, 1.0f);
        }

        vertices.push_back(vertex);
    }

    // Process indices
    for (UINT i = 0; i < in_mesh->mNumFaces; i++)
    {
        aiFace face = in_mesh->mFaces[i];
        for (UINT j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    return MeshData<Vertex>(vertices, indices);
}

Ref<Texture> D11_ModelLoader::_ProcessMaterial(aiMesh* in_mesh, const aiScene* in_scene)
{
    if (in_mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = in_scene->mMaterials[in_mesh->mMaterialIndex];
        aiString    path;
        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
        {
            std::filesystem::path texturePath = m_directory / path.C_Str();
            return Texture::Create(texturePath,
                                   D11_SamplerState::Create(eSamplerFilter::Linear, eSamplerAddress::Border));
        }
    }
    return nullptr;
}

}   // namespace crab