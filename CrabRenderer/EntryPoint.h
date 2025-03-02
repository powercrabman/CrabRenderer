#pragma once

#include "Application.h"

int main(int argc, char** argv)
{
    using namespace crab;
    Log::Init();

    // Create Command Line Args
    CommandLineArgs commandLineArgs;
    commandLineArgs.args.reserve(argc);

    for (int i = 0; i < argc; ++i)
        commandLineArgs.args.push_back(argv[i]);

    // Create Application
    Application::s_instance = Scope<Application>(::CreateCrabApplication(commandLineArgs));

    Application& app = GetApplication();
    app.OnInit();
    const int output = app._Run();
    Log::Shutdown();
    return output;
}