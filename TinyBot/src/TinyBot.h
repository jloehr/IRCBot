#pragma once

#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <ev++.h>

#include "types.h"
#include "PlugIn.h"
#include "ControlPanel.h"
#include "Server.h"

class CTinyBot
{
friend class CTinyBotFactory;
public:	
	~CTinyBot();

	void Run();
	void Stop();

	void Connect(const StringPair & Server, const StringPairVector & Channels);

private:
	CTinyBot(const std::string Botname, const StringVector & Plugins);

	const std::string m_Botname;
	CControlPanel m_ControlPanel;
	PluginVector m_Plugins;
	ServerVector m_Servers;

	void LoadPlugins(const StringVector & Plugins);

	void Init();
	void RunLoop();

	void StartWatchers();
	void StopWatchers();
};