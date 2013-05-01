#pragma once

#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>

#include "types.h"
#include "ControlPanel.h"

class CTinyBot
{
friend class CTinyBotFactory;
public:	
	~CTinyBot();

	void Run();
	void Stop();


private:
	CTinyBot(const int WorkerThreadNum, const std::string Botname);

	const int m_WorkerThreadNum;
	const std::string m_Botname;

	CControlPanel m_ControlPanel;

	void RunLoop();

	void StartWatchers();
	void StopWatchers();
};