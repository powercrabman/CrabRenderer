#include "InternalPCH.h"

#include "Application.h"

#include "AppWindow.h"
#include "CrabEvent.h"
#include "EventDispatcher.h"
#include "Input.h"
#include "SceneManager.h"
#include "Timer.h"

namespace crab
{

Scope<Application> Application::s_instance = nullptr;

//===================================================
// Application
//===================================================

Application::Application(const ApplicationCreateInfo& in_info)
    : m_isRunning(true)
    , m_applicationName(in_info.applicationName)
    , m_projectDirectories(in_info.projectDirectories)
{
    m_appWindow = CreateScope<AppWindow>();
    m_appWindow->Init(AppWindowInitInfo {
        .windowTitle  = in_info.windowTitle,
        .windowWidth  = in_info.windowWidth,
        .windowHeight = in_info.windowHeight,
        .windowPosX   = in_info.windowPosX,
        .windowPosY   = in_info.windowPosY,
        .centerAlign  = (in_info.flags & eApplicationCreateFlags_Window_CenterAlign) != 0 });

    GetRenderer().Init(RendererInitInfo {
        .backBufferFormat            = in_info.backBufferFormat,
        .depthBufferFormat = in_info.backBufferDepthBufferFormat,
        .HDRBackBufferFormat         = in_info.HDRBackBufferFormat,
        .width                       = in_info.windowWidth,
        .height                      = in_info.windowHeight,
        .createHDR                      = (in_info.flags & eApplicationCreateFlags_Renderer_EnableHDR) != 0,
        .enableMSAA                     = (in_info.flags & eApplicationCreateFlags_Renderer_EnableMSAA) != 0,
        .createDepthBuffer              = (in_info.flags & eApplicationCreateFlags_Renderer_EnableDepthBuffer) != 0,
        .vSync                       = (in_info.flags & eApplicationCreateFlags_Renderer_EnableVSync) != 0,
        .nativeHandle                        = m_appWindow->GetWindowHandle() });

    ImguiGlue::Init(
        m_appWindow->GetSDLWindow(),
        GetRenderer().GetDevice(),
        GetRenderer().GetDeviceContext());

    m_eventQueue.reserve(1000);
}

Application::~Application()
{
}

crab::AppWindow& Application::GetAppWindow() const
{
    ASSERT(m_appWindow, "AppWindow is null!");
    return *m_appWindow;
}

Application& Application::GetInstance()
{
    ASSERT(s_instance, "Application instance is null!");
    return *s_instance;
}

std::string_view Application::GetApplicationName() const
{
    return m_applicationName;
}

void Application::Init_Internal()
{
    GetRenderer().InitGlobalResources(m_projectDirectories.shaderDirectory);

    OnInit();
}

int Application::Run_Internal()
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
        m_appWindow->PollEvent_Internal();

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
            for (const auto& event: m_eventQueue)
                OnEvent_Internal(event.get());

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

void Application::OnEvent_Internal(EventBase* in_event)
{
    log::Debug(R"(Event Occurred. "{0}")", in_event->ToString());

    // - Handle Event
    EventDispatcher dispatcher { in_event };

    // - Dispatch Event
    Input::OnEvent(in_event);
    GetSceneManager().OnEvent(in_event);

    DISPATCH_EVENT(WindowEvent_WindowResize,
                   [&](WindowEvent_WindowResize* e)
                   {
                       GetRenderer().OnResize(e->GetWidth(), e->GetHeight());

                       RendererEvent_ResizeBackBuffer event;
                       DispatchEvent(event);
                   });

    DISPATCH_EVENT(ApplicationEvent_AppClose,
                   [&](ApplicationEvent_AppClose*)
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
    ApplicationEvent_AppClose event;
    DispatchEventLater(event);
}

}   // namespace crab