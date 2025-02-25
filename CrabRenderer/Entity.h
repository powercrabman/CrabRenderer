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

    template<typename... Ty>
    bool HasComponent() const
    {
        return m_registry->all_of<Ty...>(m_entity);
    }

    template<typename Ty, typename... Args>
    void CreateComponent(Args&&... in_args)
    {
        m_registry->emplace<Ty>(m_entity, std::forward<Args>(in_args)...);
    }

    template<typename Ty, typename... Args>
    Ty& GetOrCreateComponent(Args&&... in_args)
    {
        return m_registry->emplace<Ty>(m_entity, std::forward<Args>(in_args)...);
    }

    template<typename... Ty>
    void RemoveComponent() const
    {
        m_registry->remove<Ty...>(m_entity);
    }

    template<typename Ty>
    Ty& GetComponent()
    {
        return m_registry->get<Ty>(m_entity);
    }

    template<typename... Ty>
    auto GetComponents() const
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