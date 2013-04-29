#pragma once

#include <string>
#include <stdlib.h>

#include "TinyBot.h"

class CTinyBotFactory
{
public:
	static CTinyBot & CreateTinyBot(const int argc, const char* argv[]);

private:
	CTinyBotFactory(const int argc, const char* argv[]);
	~CTinyBotFactory();

	static const char * const Defaultname;
	const int m_argc;
	const char** m_argv;

	int m_WorkerThreadNum;
	std::string m_Botname;

	CTinyBot * m_Product;

	CTinyBot & Build();
	void ParseArguments();
};