#pragma once

#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>

#include "types.h"

class CTinyBot
{
friend class CTinyBotFactory;
public:	
	~CTinyBot();

	void Run();


private:
	CTinyBot(const int WorkerThreadNum, const std::string Botname);

	const int m_WorkerThreadNum;
	const std::string m_Botname;


};