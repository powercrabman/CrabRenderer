#include "CrabPch.h"

#include "Mesh.h"
#include "Model.h"

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

    uint32 flags = aiProcess_Triangulate
                   | aiProcess_CalcTangentSpace
                   | aiProcess_GenNormals
                   | aiProcess_ConvertToLeftHanded;

    const aiScene* pScene = importer.ReadFile(in_modelPath.string(), flags);

    if (!pScene)
    {
        DEBUG_BREAK(fmt::format(
                        "Failed to load mesh file: {0}\n{1}", in_modelPath.string().c_str(), importer.GetErrorString())
                        .c_str());
        return false;
    }

    m_meshFilePath = in_modelPath;
    m_modelNodes.clear();

    if (pScene->mNumMeshes > 1)
        _ProcessNode(pScene->mRootNode, pScene);
    else
        _ProcessMesh(pScene->mMeshes[0], pScene);

    return true;
}

void ModelLoader::_ProcessNode(
    aiNode*        in_node,
    const aiScene* in_scene)
{
    for (UINT i = 0; i < in_node->mNumMeshes; i++)
    {
        uint32  idx  = in_node->mMeshes[i];
        aiMesh* mesh = in_scene->mMeshes[idx];
        _ProcessMesh(mesh, in_scene);
    }

    for (UINT i = 0; i < in_node->mNumChildren; i++)
    {
        _ProcessNode(in_node->mChildren[i], in_scene);
    }
}

void ModelLoader::_ProcessMesh(
    aiMesh*        in_mesh,
    const aiScene* in_scene)
{
    ModelLoaderNode modelData;

    std::vector<VertexData>& vertexData = modelData.geometryData.subDatas;
    std::vector<uint32>&     indices    = modelData.geometryData.indices;
    vertexData.reserve(in_mesh->mNumVertices);
    indices.reserve(in_mesh->mNumFaces * 3);

    modelData.geometryData.topology = eTopology::TriangleList;

    // Vertices
    for (uint32 idx = 0; idx < in_mesh->mNumVertices; ++idx)
    {
        VertexData vertex;

        // position
        Vec3 pos;
        auto meshPos    = in_mesh->mVertices[idx];
        pos.x           = meshPos.x;
        pos.y           = meshPos.y;
        pos.z           = meshPos.z;
        vertex.position = pos;

        // normal
        if (in_mesh->HasNormals())
        {
            Vec3 normal;
            auto meshNormal = in_mesh->mNormals[idx];
            normal.x        = meshNormal.x;
            normal.y        = meshNormal.y;
            normal.z        = meshNormal.z;
            vertex.normal   = normal;
        }

        // texture coordinates
        if (in_mesh->HasTextureCoords(0))
        {
            Vec2 texCoord;
            auto meshTexCoord = in_mesh->mTextureCoords[0][idx];
            texCoord.x        = meshTexCoord.x;
            texCoord.y        = meshTexCoord.y;
            vertex.texCoord   = texCoord;
        }

        // tangent
        if (in_mesh->HasTangentsAndBitangents())
        {
            Vec3 tangent;
            auto meshTan   = in_mesh->mTangents[idx];
            tangent.x      = meshTan.x;
            tangent.y      = meshTan.y;
            tangent.z      = meshTan.z;
            vertex.tangent = tangent;

            Vec3 bitangent;
            auto meshBitan   = in_mesh->mBitangents[idx];
            bitangent.x      = meshBitan.x;
            bitangent.y      = meshBitan.y;
            bitangent.z      = meshBitan.z;
            vertex.bitangent = bitangent;
        }

        // color
        if (in_mesh->HasVertexColors(0))
        {
            Vec3 color;
            auto meshColor = in_mesh->mColors[0][idx];
            color.x        = meshColor.r;
            color.y        = meshColor.g;
            color.z        = meshColor.b;
            vertex.color   = color;
        }

        vertexData.emplace_back(vertex);
    }

    // Indices
    for (uint32 idx = 0; idx < in_mesh->mNumFaces; ++idx)
    {
        aiFace face = in_mesh->mFaces[idx];
        for (uint32 j = 0; j < face.mNumIndices; ++j)
        {
            indices.emplace_back(face.mIndices[j]);
        }
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

    modelData.name = material->GetName().C_Str();
    m_modelNodes.emplace_back(modelData);
}

void Model::Init(const std::vector<ModelNode>& in_meshNodes)
{
    m_nodes = in_meshNodes;
}

void Model::Init(const Ref<Mesh>& in_mesh, const Ref<Material>& in_material)
{
    m_nodes.clear();
    m_nodes.emplace_back(ModelNode { in_mesh, in_material, "Mesh" });
}

ModelNode* Model::FindNode(std::string_view in_name)
{
    auto it = std::ranges::find_if(m_nodes,
                                   [&](const ModelNode& node)
                                   { return node.name == in_name; });

    if (it != m_nodes.end())
    {
        return &*it;
    }
    else
    {
        DEBUG_BREAK(fmt::format(
                        "Model node not found: {0}", in_name.data())
                        .data());
        return nullptr;
    }
}

}   // namespace crab