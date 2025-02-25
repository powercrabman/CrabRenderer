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
};

class Model
{
    D11_RESOURCE_CTOR(Model);

public:
    static Ref<Model> Create(const std::vector<ModelNode>& in_meshNodes);

    const std::vector<ModelNode>& GetNodes() const { return m_nodes; }

    ModelNode GetNode(uint32 in_index) const;
    void      SetNode(uint32 in_index, const ModelNode& in_node);

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
};

class ModelLoader
{
public:
    bool Load(const std::filesystem::path& in_modelPath);

    const std::vector<ModelLoaderNode>& GetModelData() const { return m_modelNodes; }

private:
    void _ProcessNode(aiNode* in_node, const aiScene* in_scene);
    void _ProcessMesh(aiMesh* in_mesh, const aiScene* in_scene);

    std::filesystem::path        m_meshFilePath;
    std::vector<ModelLoaderNode> m_modelNodes;
};

}   // namespace crab
