#include "CrabPch.h"

#include "Material.h"

namespace crab
{
    
Ref<Material> Material::Create(const MaterialData& in_materialData)
{
    auto material    = CreateRef<Material>();
    material->m_data = in_materialData;
    return material;
}

}   // namespace crab