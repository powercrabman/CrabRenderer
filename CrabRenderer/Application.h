#pragma once
#include "CrabEvent.h"
#include "Timer.h"

int main(int argc, char** argv);

namespace crab
{

struct CrabEvent;
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

    // - Event
    template<typename Ty>
    void DispatchEvent(Ty& in_event);

    template<typename Ty>
    void DispatchEventLater(Ty& in_event);

protected:
    Application(const ApplicationSetting& in_setting);

private:
    virtual void OnInit()     = 0;   // in the derived class
    virtual void OnShutdown() = 0;   // in the derived class

    int  _Run();
    void _OnEvent(CrabEvent& in_event);

    std::string      m_applicationName;
    Scope<AppWindow> m_appWindow;
    TimeStamp        m_timeStamp;

    bool m_isRunning;

    std::vector<CrabEvent> m_eventQueue;

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

template<typename Ty>
void crab::Application::DispatchEvent(Ty& in_event)
{
    static_assert(
        static_cast<uint32>(Ty::s_staticType) > 0,
        "Event Type is not implemented.");

    // - Debug

    // - Create Event
    CrabEvent e;
    e.eventData = &in_event;
    e.type      = Ty::s_staticType;

    // - Dispatch
    _OnEvent(e);
}

template<typename Ty>
void Application::DispatchEventLater(Ty& in_event)
{
    static_assert(
        static_cast<uint32>(Ty::s_staticType) > 0,
        "Event Type is not implemented.");

    CrabEvent e;
    e.eventData = &in_event;
    e.type      = Ty::s_staticType;

    m_eventQueue.emplace_back(e);
}

}   // namespace crab

//===================================================
// Create Application
// -------------------------
// This function should be implemented in the client
//===================================================

extern crab::Application* CreateCrabApplication(const crab::CommandLineArgs& in_commandLineArgs);