#pragma once
#include "Scene3DBase.h"

class SceneHierarchy;

class DemoScene : public Scene3DBase
{
    using Super = Scene3DBase;

public:
    ~DemoScene() override;

    void Init() override;

    void OnEnter() override;
    void OnExit() override;

    void OnUpdate(TimeStamp& in_ts) override;
    void OnRender(TimeStamp& in_ts) override;
    void OnPostRender(TimeStamp& in_ts) override;
    void OnRenderGUI(TimeStamp& in_ts) override;

    void OnEvent(CrabEvent& in_event) override;

private:
    Scope<SceneHierarchy> m_sceneHierarchy;

    SkyboxRenderer m_daySkybox;
    SkyboxRenderer m_nightSkybox;
    enum class eSkyboxType
    {
        Day,
        Night,
    } m_skyboxType = eSkyboxType::Night;
};
