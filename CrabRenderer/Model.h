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
    D11_RESOURCE_CTOR(Model);

public:
    static Ref<Model> Create(const std::vector<ModelNode>& in_meshNodes);

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

private:
    void _ProcessNode(aiNode* in_node, const aiScene* in_scene);
    void _ProcessMesh(aiMesh* in_mesh, const aiScene* in_scene);

    std::filesystem::path        m_meshFilePath;
    std::vector<ModelLoaderNode> m_modelNodes;
};

}   // namespace crab
