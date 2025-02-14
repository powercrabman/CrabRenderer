#include "CrabPch.h"

#include "Scene.h"

#include "Components.h"
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
    entt::entity e = entt::entity(in_id);
    if (m_registry.valid(e))
    {
        return Entity { this, e };
    }
    else
    {
        Entity entity { this, m_registry.create(e) };
        entity.AddComponent<Transform>();
        entity.AddComponent<IDComponent>(in_id);
        return entity;
    }
}

crab::Entity Scene::CreateEntity()
{
    entt::entity e = m_registry.create();
    Entity       entity { this, e };
    entity.AddComponent<Transform>();
    entity.AddComponent<IDComponent>(uint32(e));

    return entity;
}

Entity Scene::FindEntity(uint32 in_id)
{
    Entity e { this, entt::entity(in_id) };
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
    Entity e { this, entt::entity(in_id.id) };
    if (m_registry.valid(e))
    {
        return e;
    }
    else
    {
        return Entity::s_null;
    }
}

void Scene::_Init()
{
    if (!m_isInit)
    {
        // Init something

        Init();
        m_isInit = true;
    }
}

void Scene::_OnEnter()
{
    // - Init (if not init)
    _Init();

    OnEnter();
}

void Scene::_OnExit()
{
    OnExit();

    // Shutdown something
}

void Scene::_OnUpdate(TimeStamp& in_ts)
{
    // script update
    GetView<ScriptComponent>().each(
        [&](ScriptComponent& in_script)
        {
            in_script.script->OnUpdate(in_ts);
        });

    // update derived scene
    OnUpdate(in_ts);
}

void Scene::_OnRender(TimeStamp& in_ts)
{
    // render derived scene
    OnRender(in_ts);
}

void Scene::_OnEvent(CrabEvent& in_event)
{
    // pass event to derived scene
    OnEvent(in_event);
}

}   // namespace crab
