#pragma once

class DemoApp : public Application
{
public:
	DemoApp() = default;
	~DemoApp() = default;

	ApplicationSetting ConfigureApplication() override;

	void OnInit() override;
	void OnShutdown() override;
};

CRAB_ENTRY_POINT(DemoApp);
