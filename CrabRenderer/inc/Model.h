#pragma once
#include "CrabVertex.h"
#include "Material.h"

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
struct aiString;

namespace crab
{
class Mesh;
class Material;

//===================================================
// Model
//===================================================

struct ModelNode
{
    Ref<Mesh>     mesh;
    Ref<Material> material;

    std::string name;
};

class Model
{
public:
    void Init(const std::vector<ModelNode>& in_meshNodes);
    void Init(const Ref<Mesh>& in_mesh, const Ref<Material>& in_material);

    const std::vector<ModelNode>& GetNodes() const { return m_nodes; }
    std::vector<ModelNode>&       GetNodesRef() { return m_nodes; }

    ModelNode* FindNode(std::string_view in_name);

private:
    std::vector<ModelNode> m_nodes;
};

//===================================================
// Model Loader
//===================================================

struct ModelLoaderNode
{
    GeometryData geometryData;
    MaterialData materialData;

    std::string name;
};

class ModelLoader
{
public:
    bool Load(const std::filesystem::path& in_modelPath);

    const std::vector<ModelLoaderNode>& GetModelData() const { return m_modelNodes; }

    template<class VertexType>
    NODISCARD Ref<Model> CreateModel(const std::function<VertexType(const VertexData&)>& in_vertexCreationFunc);

private:
    void _ProcessNode(aiNode* in_node, const aiScene* in_scene);
    void _ProcessMesh(aiMesh* in_mesh, const aiScene* in_scene);

    std::filesystem::path        m_meshFilePath;
    std::vector<ModelLoaderNode> m_modelNodes;
};

template<typename VertexType>
Ref<Model> ModelLoader::CreateModel(const std::function<VertexType(const VertexData&)>& in_vertexCreationFunc)
{
    std::vector<ModelNode> modelNodes;

    for (const auto& node: m_modelNodes)
    {
        std::vector<VertexType> vertices;
        vertices.reserve(node.geometryData.subDatas.size());

        for (const auto& vertexData: node.geometryData.subDatas)
        {
            vertices.push_back(in_vertexCreationFunc(vertexData));
        }

        Ref<Mesh> mesh = CreateRef<Mesh>();
        mesh->Init(vertices, node.geometryData.indices, node.geometryData.topology);

        Ref<Material> material = CreateRef<Material>();
        material->Init(node.materialData);

        modelNodes.push_back(ModelNode { mesh, material, node.name });
    }

    Ref<Model> model = CreateRef<Model>();
    model->Init(modelNodes);
    return model;
}

}   // namespace crab
