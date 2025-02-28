#pragma once

class PostProcessingDemo2 : public Scene
{
public:
    void Init() override;
    void OnEnter() override;
    void OnExit() override;

    void OnUpdate(TimeStamp& in_ts) override;
    void OnRender(TimeStamp& in_ts) override;
    void OnRenderGUI(TimeStamp& in_ts) override;

    void OnEvent(CrabEvent& in_event) override;

    IMPLEMENT_SCENE(PostProcessingDemo2);

private:
    std::vector<Ref<PostProcess>> m_postProcessStack;

    float m_resolutionX;
    float m_resolutionY;
    float m_deltaTime;
    Vec4  g_userValues;
};
