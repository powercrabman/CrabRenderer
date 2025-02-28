#pragma once

inline Ref<Model> CreateModel(const std::filesystem::path& in_path)
{
    ModelLoader loader;
    if (loader.Load(in_path))
    {
        auto nodes = loader.GetModelData();

        std::vector<ModelNode> modelNodes;
        modelNodes.reserve(nodes.size());
        for (const auto& node: nodes)
        {
            ModelNode modelNode;
            modelNode.mesh = Mesh::Create(
                Vertex3D::CreateVertices(node.geometryData),
                node.geometryData.indices,
                eTopology::TriangleList);
            modelNode.material = Material::Create(node.materialData);
            modelNode.name     = node.name;
            modelNodes.push_back(modelNode);
        }
        return Model::Create(modelNodes);
    }
    else
    {
        CRAB_DEBUG_BREAK("Failed to load model");
        return nullptr;
    }
}