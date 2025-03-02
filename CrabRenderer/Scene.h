#pragma once
#include "Entity.h"

namespace crab
{

struct TimeStamp;

class Scene
{
    friend class SceneManager;

public:
    virtual ~Scene() = default;

    // pure virtual
    virtual void Init() = 0;

    virtual void OnEnter() = 0;
    virtual void OnExit()  = 0;

    virtual void OnUpdate(TimeStamp& in_ts)     = 0;
    virtual void OnRender(TimeStamp& in_ts)     = 0;
    virtual void OnPostRender(TimeStamp& in_ts) = 0;
    virtual void OnRenderGUI(TimeStamp& in_ts)  = 0;

    virtual void OnEvent(CrabEvent& in_event) = 0;

    // getter
    const TypeInfo& GetTypeInfo() const { return m_sceneTypeInfo; }
    const char*     GetName() const { return m_sceneName.c_str(); }
    bool            IsInited() const { return m_isInit; }

    // entt
    entt::registry&       GetRegistry();
    const entt::registry& GetRegistry() const { return m_registry; }

    Entity CreateEntity();
    Entity CreateEntity(std::string_view in_tag);
    Entity CreateEntity(uint32 in_id);
    Entity CloneEntity(Entity in_src);

    Entity FindEntity(uint32 in_id);
    Entity FindEntity(const IDComponent& in_id);

    template<class... Types, class... Exclude>
    auto GetView(entt::exclude_t<Exclude...> exclude = entt::exclude_t {});
    void ClearRegistry();

protected:
    Scene() = default;

private:
    Entity _CreateEntity(entt::entity e);

    bool           m_isInit = false;
    entt::registry m_registry;

    TypeInfo    m_sceneTypeInfo = {};
    std::string m_sceneName     = {};
};

inline Entity Scene::_CreateEntity(entt::entity e)
{
    Entity entity = { this, e };
    entity.CreateComponent<TransformComponent>();
    entity.CreateComponent<IDComponent>(static_cast<uint32>(e));
    return entity;
}

//===================================================
// Inline
//===================================================

template<typename... Types, typename... Exclude>
auto Scene::GetView(entt::exclude_t<Exclude...> exclude)
{
    return m_registry.view<Types...>(exclude);
}

}   // namespace crab