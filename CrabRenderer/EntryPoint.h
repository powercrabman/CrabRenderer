#pragma once

#define CRAB_ENTRY_POINT(AppClass)                                             \
    int main(int argc, char* argv[])                                           \
    {                                                                          \
        crab::Application::s_instance            = CreateScope<AppClass>();    \
        crab::Application&               app     = crab::GetApplication();     \
        crab::ApplicationSetting         setting = app.ConfigureApplication(); \
        crab::ApplicationCommandLineArgs args    = { argc, argv };             \
        app._Init(setting, args);                                              \
        return app._Run();                                                     \
    }