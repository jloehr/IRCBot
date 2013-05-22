#pragma once

#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>

#include "types.h"
#include "ControlPanel.h"
#include "Server.h"

class CTinyBot
{
friend class CTinyBotFactory;
public:	
	~CTinyBot();

	void Run();
	void Stop();

	void Connect(StringPair * Server, StringPairVector * Channels);

private:
	CTinyBot(const std::string Botname);

	const std::string m_Botname;
	CControlPanel m_ControlPanel;

	ServerVector m_Servers;

	void RunLoop();

	void StartWatchers();
	void StopWatchers();
};