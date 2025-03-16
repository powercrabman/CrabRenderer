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

Entity Scene::CloneEntity(Entity in_src)
{
    Entity e         = CreateEntity();
    e.GetTransform() = in_src.GetTransform();

    auto storage = m_registry.storage();

    for (auto& set: storage | std::views::values)
    {
        if (set.contains(in_src) && !set.contains(e))
            set.push(e, set.value(in_src));
    }

    return e;
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

Entity Scene::FindEntityByID(uint32 in_id)
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

Entity Scene::FindEntityByID(const IDComponent& in_id)
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

Entity Scene::FindEntityByTag(std::string_view in_tag)
{
    auto view = m_registry.view<TagComponent>();
    for (auto e: view)
    {
        if (view.get<TagComponent>(e) == in_tag)
        {
            return Entity { this, e };
        }
    }

    return Entity::s_null;
}

void Scene::ClearRegistry()
{
    m_registry.clear();
}

Entity Scene::FindEntityByTag(const TagComponent& in_tag)
{
    auto view = m_registry.view<TagComponent>();
    for (auto e: view)
    {
        if (view.get<TagComponent>(e) == in_tag)
        {
            return Entity { this, e };
        }
    }

    return Entity::s_null;
}

}   // namespace crab
