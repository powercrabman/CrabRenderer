#include "CrabPch.h"

#include "Entity.h"

#include "Scene.h"
#include "Components.h"

namespace crab
{

crab::Entity Entity::s_null = Entity {};

Entity::Entity(Scene* in_scene, entt::entity in_entity)
    : m_registry(&in_scene->GetRegistry())
    , m_entity(in_entity)
{
}

Entity::Entity()
{
    m_entity = entt::null;
    m_registry = nullptr;
}

crab::IDComponent& Entity::GetID() 
{
    return GetComponent<IDComponent>();
}

crab::Transform& Entity::GetTransform()
{
    return GetComponent<Transform>();
}

}   // namespace crab