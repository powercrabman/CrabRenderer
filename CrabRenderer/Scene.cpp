#include "CrabPch.h"

#include "Scene.h"

#include "CrabComponents.h"
#include "Entity.h"
#include "Script.h"

namespace crab
{

entt::registry& Scene::GetRegistry()
{
    return m_registry;
}

crab::Entity Scene::CreateEntity(uint32 in_id)
{
    entt::entity e = static_cast<entt::entity>(in_id);
    if (m_registry.valid(e))
    {
        return Entity { this, e };
    }
    else
    {
        return _CreateEntity(m_registry.create(e));
    }
}

crab::Entity Scene::CreateEntity()
{
    return _CreateEntity(m_registry.create());
}

Entity Scene::CreateEntity(std::string_view in_tag)
{
    Entity entity = _CreateEntity(m_registry.create());
    entity.CreateComponent<TagComponent>(std::string(in_tag));

    return entity;
}

Entity Scene::FindEntity(uint32 in_id)
{
    Entity e { this, static_cast<entt::entity>(in_id) };
    if (m_registry.valid(e))
    {
        return e;
    }
    else
    {
        return Entity::s_null;
    }
}

Entity Scene::FindEntity(const IDComponent& in_id)
{
    Entity e { this, static_cast<entt::entity>(in_id.id) };
    if (m_registry.valid(e))
    {
        return e;
    }
    else
    {
        return Entity::s_null;
    }
}


}   // namespace crab
