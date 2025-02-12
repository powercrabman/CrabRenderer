#pragma once

class DemoApp : public Application
{
public:
	DemoApp();
	~DemoApp();

	ApplicationSetting ConfigureApplication() override;

	void OnInit() override;
	void OnShutdown() override;
};

CRAB_ENTRY_POINT(DemoApp);
