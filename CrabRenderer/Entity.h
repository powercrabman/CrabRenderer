#pragma once

namespace crab
{

class Scene;
struct IDComponent;
struct TransformComponent;

class Entity
{
public:
    Entity();
    Entity(Scene* in_scene, entt::entity in_entity);
    operator entt::entity() const { return m_entity; }

    IDComponent&        GetID();
    TransformComponent& GetTransform();

    void Destroy() const
    {
        m_registry->destroy(m_entity);
    }

    template<typename Ty>
    NODISCARD bool HasComponent() const
    {
        return m_registry->all_of<Ty>(m_entity);
    }

    template<typename... Ty>
    NODISCARD bool HaveComponents() const
    {
        return m_registry->all_of<Ty...>(m_entity);
    }

    template<typename ... Ty>
    NODISCARD bool HasAnyOfComponent() const
    {
        return m_registry->any_of<Ty...>(m_entity);
    }

    template<typename Ty, typename... Args>
    void CreateComponent(Args&&... in_args)
    {
        m_registry->emplace<Ty>(m_entity, std::forward<Args>(in_args)...);
    }

    template<typename Ty, typename... Args>
    void ReplaceComponent(Args&&... in_args)
    {
        m_registry->replace<Ty>(m_entity, std::forward<Args>(in_args)...);
    }

    template<typename... Ty>
    void RemoveComponent() const
    {
        m_registry->remove<Ty...>(m_entity);
    }

    template<typename Ty>
    NODISCARD Ty& GetComponent()
    {
        return m_registry->get<Ty>(m_entity);
    }

    template<typename... Ty>
    NODISCARD auto GetComponents() const
    {
        return m_registry->get<Ty...>(m_entity);
    }

    template<typename Ty>
    NODISCARD const Ty& GetComponent() const
    {
        return m_registry->get<Ty>(m_entity);
    }

    NODISCARD bool IsValid() const
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