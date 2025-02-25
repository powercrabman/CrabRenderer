#include "CrabPch.h"

#include "Application.h"

#include "AppWindow.h"
#include "CrabEvent.h"
#include "D11Renderer.h"
#include "EventDispatcher.h"
#include "ILayer.h"
#include "Input.h"
#include "SceneManager.h"
#include "Timer.h"

namespace crab
{

Scope<Application> Application::s_instance = nullptr;

//===================================================
// Application
//===================================================

Application::Application()
    : m_applicationName(std::string {})
    , m_appWindow(CreateScope<AppWindow>())
    , m_isRunning(true)
{
}

Application::~Application()
{
}

crab::AppWindow& Application::GetAppWindow()
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

void Application::_Init(const ApplicationSetting&         in_setting,
                        const ApplicationCommandLineArgs& in_args)
{
    // - Log
    Log::Init(in_setting.logSetting);

    // - Application Command Line
    auto args = _SplitComandLineArgs(in_args);
    _TranslateCommandLineArgs(args);

    // - Init Engine Modules
    m_applicationName = in_setting.applicationName;
    m_appWindow->Init(in_setting.windowSetting);
    Input::Init();
    GetRenderer().Init(in_setting.rendererSetting);

    // - Init Derived Application
    OnInit();
}

int Application::_Run()
{
    Timer timer;
    timer.Reset();

    SceneManager& sc = GetSceneManager();

    while (true)
    {
        // window poll event
        m_appWindow->_PollEvent();

        // running check
        if (!m_isRunning)
            break;

        // update timer
        timer.Tick();
        m_timeStamp = timer.GetTimeStamp();

        // update scene
        sc.OnUpdate(m_timeStamp);

        // render
        sc.OnRender(m_timeStamp);

        GetRenderer().BeginGUI();
        sc.OnRenderGUI(m_timeStamp);
        GetRenderer().EndGUI();

        // end
        GetRenderer().Present();

        // update input
        Input::Update();
    }

    // shutdown
    OnShutdown();
    Log::Shutdown();

    return 0;
}

//===================================================
// - Private
//===================================================

std::vector<std::string> Application::_SplitComandLineArgs(const ApplicationCommandLineArgs& in_args)
{
    std::vector<std::string> result;
    result.reserve(in_args.argc);

    for (int i = 0; i < in_args.argc; ++i)
    {
        result.push_back(in_args.argv[i]);
    }
    return result;
}

void Application::_TranslateCommandLineArgs(const std::vector<std::string>& in_args)
{
    // Do something
    for (const auto& arg: in_args)
    {
        std::printf("%s\n", arg.c_str());
    }
}

void Application::_OnEvent(CrabEvent& in_event)
{
    // - Handle Event
    EventDispatcher dispatcher { in_event };

    HANDLE_EVENT(AppClose_CoreEvent,
                 [&](const AppClose_CoreEvent& in_event)
                 {
                     m_isRunning = false;
                 });

    // - Dispatch Event
    Input::OnEvent(in_event);
    GetSceneManager().OnEvent(in_event);
}

const crab::TimeStamp& Application::GetTimeStamp() const
{
    return m_timeStamp;
}

void Application::Quit()
{
    AppClose_CoreEvent event {};
    DispatchEvent(event);
}

}   // namespace crab