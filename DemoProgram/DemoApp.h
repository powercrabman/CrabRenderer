#pragma once

class DemoApp : public Application
{
public:
    DemoApp();
    ~DemoApp() override;

    ApplicationSetting ConfigureApplication() override;

    void OnInit() override;
    void OnShutdown() override;

    void PreLoadResources();
};

CRAB_ENTRY_POINT(DemoApp);
