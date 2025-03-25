#pragma once
#include "EngineConfigs.h"
#include "Timer.h"

int main(int argc, char** argv);

namespace crab
{

class EventBase;
class AppWindow;

//===================================================
// Command Line Arguments
//===================================================

struct CommandLineArgs
{
    std::vector<std::string> args;
};

//===================================================
// Application
//===================================================

class Application
{
    friend int ::main(int argc, char** argv);

public:
    virtual ~Application();

    void Quit();

    // - Getter
    AppWindow&          GetAppWindow() const;
    static Application& GetInstance();
    std::string_view    GetApplicationName() const;
    const TimeStamp&    GetTimeStamp() const;

    const ProjectDirectories& GetProjectDirectories() const { return m_projectDirectories; }

    // - Event
    template<typename Ty>
    void DispatchEvent(Ty& in_event);

    template<typename Ty>
    void DispatchEventLater(Ty& in_event);

protected:
    Application(const ApplicationCreateInfo& in_info);

private:
    virtual void OnInit()     = 0;   // in the derived class
    virtual void OnShutdown() = 0;   // in the derived class

    void Init_Internal();
    int  Run_Internal();
    void OnEvent_Internal(EventBase* in_event);

    std::string      m_applicationName;
    Scope<AppWindow> m_appWindow;
    TimeStamp        m_timeStamp;

    ProjectDirectories m_projectDirectories;

    bool m_isRunning;

    std::vector<Scope<EventBase>> m_eventQueue;

    // - Singleton
    static Scope<Application> s_instance;
};

//===================================================
// Inline
//===================================================

inline Application& GetApplication()
{
    return Application::GetInstance();
}

inline AppWindow& GetAppWindow()
{
    return GetApplication().GetAppWindow();
}

template<typename EventType>
void Application::DispatchEvent(EventType& in_event)
{
    static_assert(IS_BASE_OF(EventBase, EventType),
                  "Event Type is not derived from EventBase.");

    // - Dispatch
    OnEvent_Internal(&in_event);
}

template<typename EventType>
void Application::DispatchEventLater(EventType& in_event)
{
    static_assert(IS_BASE_OF(EventBase, EventType),
                  "Event Type is not derived from EventBase.");

    m_eventQueue.emplace_back(CreateScope<EventType>(in_event));
}

}   // namespace crab

//===================================================
// Create Application
// -------------------------
// This function should be implemented in the client
//===================================================

extern crab::Application* CreateCrabApplication(const crab::CommandLineArgs& in_commandLineArgs);