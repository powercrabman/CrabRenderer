#include "CrabPch.h"

#include "Application.h"

#include "AppWindow.h"
#include "CrabEvent.h"
#include "D11Renderer.h"
#include "EventDispatcher.h"
#include "ImguiGlue.h"
#include "Input.h"
#include "SceneManager.h"
#include "Timer.h"

namespace crab
{

Scope<Application> Application::s_instance = nullptr;

//===================================================
// Application
//===================================================

Application::Application(const ApplicationSetting& in_setting)
    : m_applicationName(in_setting.applicationName)
    , m_appWindow(CreateScope<AppWindow>())
    , m_isRunning(true)
{
    m_appWindow->Init(in_setting.windowSetting);

    GetRenderer().Init(
        in_setting.rendererSetting,
        in_setting.windowSetting.windowSize,
        m_appWindow->GetWindowHandle());

    ImguiGlue::Init(
        m_appWindow->GetSDLWindow(),
        GetRenderer().GetDevice(),
        GetRenderer().GetContext());

    m_eventQueue.reserve(1000);
}

Application::~Application()
{
}

crab::AppWindow& Application::GetAppWindow() const
{
    CRAB_ASSERT(m_appWindow, "AppWindow is null!");
    return *m_appWindow;
}

Application& Application::GetInstance()
{
    CRAB_ASSERT(s_instance, "Application instance is null!");
    return *s_instance;
}

std::string_view Application::GetApplicationName() const
{
    return m_applicationName;
}

int Application::_Run()
{
    Timer timer;
    timer.Reset();

    SceneManager& sc = GetSceneManager();

    while (true)
    {
        // running check
        if (!m_isRunning)
            break;

        // window poll event
        m_appWindow->_PollEvent();

        // update timer
        timer.Tick();
        m_timeStamp = timer.GetTimeStamp();

        // update scene
        sc.OnUpdate(m_timeStamp);

        // render
        sc.OnRender(m_timeStamp);

        ImguiGlue::BeginRender();
        sc.OnRenderGUI(m_timeStamp);
        ImguiGlue::EndRender();

        // end
        GetRenderer().Present();

        // update input
        Input::Update();

        // defer event dispatch
        if (!m_eventQueue.empty())
        {
            for (auto& event: m_eventQueue)
                _OnEvent(event);

            m_eventQueue.clear();
        }
    }

    // shutdown
    OnShutdown();
    ImguiGlue::Shutdown();

    return 0;
}

//===================================================
// - Private
//===================================================

void Application::_OnEvent(CrabEvent& in_event)
{
    Log::Debug(R"(Event Occurred. "{0}")", magic_enum::enum_name(in_event.type));

    // - Handle Event
    EventDispatcher dispatcher { in_event };

    // - Dispatch Event
    Input::OnEvent(in_event);
    GetSceneManager().OnEvent(in_event);

    HANDLE_EVENT(AppClose_CoreEvent,
                 [&](const AppClose_CoreEvent& in_event)
                 {
                     m_isRunning = false;
                 });
}

const crab::TimeStamp& Application::GetTimeStamp() const
{
    return m_timeStamp;
}

void Application::Quit()
{
    AppClose_CoreEvent event {};
    DispatchEventLater(event);
}

}   // namespace crab