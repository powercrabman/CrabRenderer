#include "CrabPch.h"

#include "Mesh.h"
#include "Model.h"
#include "RenderStates.h"
#include "Textures.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <DirectXMesh.h>
#include <DirectXMesh.inl>

namespace crab
{

//===================================================
// Model Loader
//===================================================

bool ModelLoader::Load(const std::filesystem::path& in_modelPath)
{
    Assimp::Importer importer;

    uint32 flags = aiProcess_CalcTangentSpace
                   | aiProcess_Triangulate
                   | aiProcess_ConvertToLeftHanded;

    const aiScene* pScene = importer.ReadFile(in_modelPath.string(), flags);

    if (!pScene)
    {
        CRAB_DEBUG_BREAK_V("Failed to load mesh file: {0}", in_modelPath.string().c_str());
        return false;
    }

    m_meshFilePath = in_modelPath;
    m_modelNodes.clear();

    _ProcessNode(pScene->mRootNode, pScene);

    return true;
}

void ModelLoader::_ProcessNode(aiNode* in_node, const aiScene* in_scene)
{
    for (UINT i = 0; i < in_node->mNumMeshes; i++)
    {
        aiMesh* mesh = in_scene->mMeshes[in_node->mMeshes[i]];
        _ProcessMesh(mesh, in_scene);
    }

    for (UINT i = 0; i < in_node->mNumChildren; i++)
    {
        _ProcessNode(in_node->mChildren[i], in_scene);
    }
}

void ModelLoader::_ProcessMesh(aiMesh* in_mesh, const aiScene* in_scene)
{
    ModelLoaderNode modelData;

    GeometryData& vertexData = modelData.geometryData;
    vertexData.colors.reserve(in_mesh->mNumVertices);
    vertexData.normals.reserve(in_mesh->mNumVertices);
    vertexData.positions.reserve(in_mesh->mNumVertices);
    vertexData.texCoords.reserve(in_mesh->mNumVertices);
    vertexData.tangents.reserve(in_mesh->mNumVertices);
    vertexData.indices.reserve(in_mesh->mNumFaces * 3);
    vertexData.topology = eTopology::TriangleList;

    // Vertices
    for (uint32 idx = 0; idx < in_mesh->mNumVertices; ++idx)
    {
        // position
        Vec3 pos;
        auto meshPos = in_mesh->mVertices[idx];
        pos.x        = meshPos.x;
        pos.y        = meshPos.y;
        pos.z        = meshPos.z;
        vertexData.positions.emplace_back(pos);

        // normal
        if (in_mesh->HasNormals())
        {
            Vec3 normal;
            auto meshNormal = in_mesh->mNormals[idx];
            normal.x        = meshNormal.x;
            normal.y        = meshNormal.y;
            normal.z        = meshNormal.z;
            vertexData.normals.emplace_back(normal);
        }
        else
        {
            vertexData.normals.emplace_back(Vec3 { 0.0f, 0.0f, 0.0f });
        }

        // texture coordinates
        if (in_mesh->HasTextureCoords(0))
        {
            Vec2 texCoord;
            auto meshTexCoord = in_mesh->mTextureCoords[0][idx];
            texCoord.x        = meshTexCoord.x;
            texCoord.y        = meshTexCoord.y;
            vertexData.texCoords.emplace_back(texCoord);
        }
        else
        {
            vertexData.texCoords.emplace_back(Vec2 { 0.0f, 0.0f });
        }

        // tangent
        if (in_mesh->HasTangentsAndBitangents())
        {
            Vec3 tangent;
            auto meshTan = in_mesh->mTangents[idx];
            tangent.x    = meshTan.x;
            tangent.y    = meshTan.y;
            tangent.z    = meshTan.z;
            vertexData.tangents.emplace_back(tangent);
        }

        // color
        if (in_mesh->HasVertexColors(0))
        {
            Vec3 color;
            auto meshColor = in_mesh->mColors[0][idx];
            color.x        = meshColor.r;
            color.y        = meshColor.g;
            color.z        = meshColor.b;
            vertexData.colors.emplace_back(color);
        }
        else
        {
            vertexData.colors.emplace_back(Vec3 { 1.0f, 1.0f, 1.0f });
        }
    }

    // Indices
    for (uint32 idx = 0; idx < in_mesh->mNumFaces; ++idx)
    {
        aiFace face = in_mesh->mFaces[idx];
        for (uint32 j = 0; j < face.mNumIndices; ++j)
        {
            vertexData.indices.emplace_back(face.mIndices[j]);
        }
    }

    // Compute something
    if (vertexData.normals.empty())
    {
        vertexData.normals.resize(vertexData.positions.size());
        DirectX::ComputeNormals(
            vertexData.indices.data(),
            vertexData.indices.size() / 3,
            vertexData.positions.data(),
            vertexData.positions.size(),
            DirectX::CNORM_DEFAULT,
            vertexData.normals.data());
    }

    if (vertexData.tangents.empty())
    {
        vertexData.tangents.resize(vertexData.positions.size());
        DirectX::ComputeTangentFrame(
            vertexData.indices.data(),
            vertexData.indices.size() / 3,
            vertexData.positions.data(),
            vertexData.normals.data(),
            nullptr,
            vertexData.positions.size(),
            vertexData.tangents.data(),
            nullptr);
    }

    // Material
    // do not texture loading just value (e.g. color)
    // if you want to load texture, you should hand-operate

    MaterialData& materialData = modelData.materialData;
    aiMaterial*   material     = in_scene->mMaterials[in_mesh->mMaterialIndex];

    {
        aiColor3D color = {};
        if (material->Get(AI_MATKEY_BASE_COLOR, color) == AI_SUCCESS)
        {
            materialData.baseColor.x = color.r;
            materialData.baseColor.y = color.g;
            materialData.baseColor.z = color.b;
        }
    }

    {
        aiColor3D color = {};
        if (material->Get(AI_MATKEY_EMISSIVE_INTENSITY, color) == AI_SUCCESS)
        {
            materialData.emissive.x = color.r;
            materialData.emissive.y = color.g;
            materialData.emissive.z = color.b;
        }
    }

    {
        float metallic = 0.0f;
        if (material->Get(AI_MATKEY_METALLIC_FACTOR, metallic) == AI_SUCCESS)
        {
            materialData.metallic = metallic;
        }
    }

    {
        float roughness = 0.0f;
        if (material->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness) == AI_SUCCESS)
        {
            materialData.roughness = roughness;
        }
    }

    {
        aiColor3D color = {};
        if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
        {
            materialData.diffuse.x = color.r;
            materialData.diffuse.y = color.g;
            materialData.diffuse.z = color.b;
        }
    }

    {
        aiColor3D color = {};
        if (material->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
        {
            materialData.specular.x = color.r;
            materialData.specular.y = color.g;
            materialData.specular.z = color.b;
        }
    }

    {
        aiColor3D color = {};
        if (material->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
        {
            materialData.ambient.x = color.r;
            materialData.ambient.y = color.g;
            materialData.ambient.z = color.b;
        }
    }

    {
        float sharpness = 0.0f;
        if (material->Get(AI_MATKEY_SHININESS, sharpness) == AI_SUCCESS)
        {
            materialData.shininess = sharpness;
        }
    }

    m_modelNodes.emplace_back(modelData);
}

ModelNode Model::GetNode(uint32 in_index) const
{
    if (in_index < m_nodes.size())
    {
        return m_nodes[in_index];
    }
    else
    {
        return {};
    }
}

Ref<Model> Model::Create(const std::vector<ModelNode>& in_meshNodes)
{
    Ref<Model> model = CreateRef<Model>();
    model->m_nodes   = in_meshNodes;

    return model;
}

void Model::SetNode(uint32 in_index, const ModelNode& in_node)
{
    // Resize
    if (in_index >= m_nodes.size())
        m_nodes.resize(in_index + 1);

    m_nodes[in_index] = in_node;
}

}   // namespace crab