#pragma once

#include "Application.h"

int main(int argc, char** argv)
{
    using namespace crab;
    // Create Command Line Args
    CommandLineArgs commandLineArgs;
    commandLineArgs.args.reserve(argc);

    for (int i = 0; i < argc; ++i)
        commandLineArgs.args.push_back(argv[i]);

    // Create Application
    Application::s_instance = Scope<Application>(::CreateCrabApplication(commandLineArgs));

    Application& app = GetApplication();
    app.Init_Internal();
    const int output = app.Run_Internal();
    return output;
}