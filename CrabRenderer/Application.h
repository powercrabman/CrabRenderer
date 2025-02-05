#pragma once
#include "Timer.h"
#include <SDL2/SDL_main.h>

namespace crab
{

struct CrabEvent;
class AppWindow;
class ILayer;

struct ApplicationCommandLineArgs
{
    int    argc;
    char** argv;
};

//===================================================
// Application
//===================================================

class Application
{
    friend int ::main(int argc, char* argv[]);

public:
    Application();
    ~Application();

    // - Getter
    AppWindow&          GetAppWindow();
    static Application& GetInstance();
    std::string_view    GetApplicationName() const;
    const TimeStamp&    GetTimeStamp() const;

    // - Event
    template<typename Ty>
    void DispatchEvent(Ty& in_event);

    // - Virtual
    virtual ApplicationSetting ConfigureApplication() = 0;
    virtual void               OnInit()               = 0;   // in the derived class
    virtual void               OnShutdown()           = 0;   // in the derived class

private:
    // - Core
    void _Init(const ApplicationSetting&         in_setting,
               const ApplicationCommandLineArgs& in_args);
    int  _Run();

    // - Command Line
    std::vector<std::string> _SplitComandLineArgs(const ApplicationCommandLineArgs& in_args);
    void                     _TranslateCommandLineArgs(const std::vector<std::string>& in_args);

    // - Event
    void _OnEvent(CrabEvent& in_event);

private:
    std::string      m_applicationName;
    Scope<AppWindow> m_appWindow;
    TimeStamp        m_timeStamp;

    bool m_isRunning;

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

template<typename Ty>
void crab::Application::DispatchEvent(Ty& in_event)
{
    static_assert((uint32)Ty::s_staticType > 0, "Event Type is not implemented.");

    // - Debug
    Log::Debug(R"(Event Occurred. "{0}")", Ty::s_staticName);

    // - Create Event
    CrabEvent e;
    e.eventData = &in_event;
    e.type      = Ty::s_staticType;

    // - Dispatch
    _OnEvent(e);
}

// Legacy Code (maybe use)
//
// void PushLayer(Scope<ILayer>&& in_layer);
// void PopLayer(const TypeInfo& in_layer);
//
// LayerStack       m_layerStack; (unused)

}   // namespace crab