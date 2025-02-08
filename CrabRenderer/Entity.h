#pragma once

namespace crab
{

class Scene;
struct IDComponent;
struct Transform;

class Entity
{
public:
    Entity();
    Entity(Scene* in_scene, entt::entity in_entity);
    operator entt::entity() const { return m_entity; }
    IDComponent& GetID();
    Transform&   GetTransform();

    void Destroy()
    {
        m_registry->destroy(m_entity);
    }

    template<typename... Ty>
    bool HasComponent() const
    {
        return m_registry->all_of<Ty...>(m_entity);
    }

    template<typename Ty, typename... Args>
    Ty& AddComponent(Args&&... in_args)
    {
        return m_registry->emplace<Ty>(m_entity, std::forward<Args>(in_args)...);
    }

    template<typename Ty, typename... Args>
    void ReplaceComponent(Args&&... in_args)
    {
        m_registry->replace<Ty>(m_entity, std::forward<Args>(in_args)...);
    }

    template<typename... Ty>
    void RemoveComponent()
    {
        m_registry->remove<Ty...>(m_entity);
    }

    template<typename Ty>
    Ty& GetComponent()
    {
        return m_registry->get<Ty>(m_entity);
    }

    template<typename... Ty>
    auto GetComponents()
    {
        return m_registry->get<Ty...>(m_entity);
    }

    template<typename Ty>
    const Ty& GetComponent() const
    {
        return m_registry->get<Ty>(m_entity);
    }

    bool IsValid() const
    {
        if (m_registry == nullptr)
            return false;
        else
            return m_registry->valid(m_entity);
    }

    static Entity s_null;

private:
    entt::entity    m_entity   = entt::null;
    entt::registry* m_registry = nullptr;
};

}   // namespace crab