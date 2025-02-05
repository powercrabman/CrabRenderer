#pragma once
#include "Entity.h"

namespace crab
{

struct TimeStamp;

class Scene
{
    friend class SceneManager;

public:
    Scene()  = default;
    ~Scene() = default;

    // pure virtual
    virtual void Init() = 0;

    virtual void OnEnter() = 0;
    virtual void OnExit()  = 0;

    virtual void OnUpdate(TimeStamp& in_ts)    = 0;
    virtual void OnRender(TimeStamp& in_ts)    = 0;
    virtual void OnRenderGUI(TimeStamp& in_ts) = 0;

    virtual void OnEvent(CrabEvent& in_event) = 0;

    // getter
    virtual const TypeInfo& GetTypeInfo() const = 0;
    virtual const char*     GetName() const     = 0;

    // entt
    entt::registry&       GetRegistry();
    const entt::registry& GetRegistry() const { return m_registry; }

    Entity CreateEntity();
    Entity CreateEntity(uint32 in_id);

    template<typename... Types>
    auto GetView();

private:
    // Internal calls
    virtual void _Init() final;

    virtual void _OnEnter() final;
    virtual void _OnExit() final;

    virtual void _OnUpdate(TimeStamp& in_ts) final;
    virtual void _OnRender(TimeStamp& in_ts) final;

    virtual void _OnEvent(CrabEvent& in_event) final;

    bool           m_isInit = false;
    entt::registry m_registry;
};

//===================================================
// Inline
//===================================================

template<typename... Types>
auto Scene::GetView()
{
    return m_registry.view<Types...>();
}

#define IMPLEMENT_SCENE(Ty)                       \
public:                                           \
    const TypeInfo& GetTypeInfo() const override  \
    {                                             \
        return ::GetTypeInfo<Ty>();               \
    }                                             \
    const char* GetName() const override          \
    {                                             \
        return #Ty;                               \
    }                                             \
    inline static const char* s_staticName = #Ty

}   // namespace crab