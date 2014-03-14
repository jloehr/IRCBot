/*
 *
 * Copyright (C) 2013 Julian Löhr
 * All rights reserved.
 *
 * This file is part of TinyBot, an IRC Bot.
 * TinyBot is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
 * To read the full license, please check the LICENSE file.
 *
*/
#pragma once

#include <cstring>
#include <string>
#include <stdlib.h>

#include "TinyBot.h"
#include "types.h"

#define DEFAULT_PORT "6667"

class CTinyBotFactory
{
public:
	static CTinyBot * CreateTinyBot(const int argc, const char* argv[]);

private:
	CTinyBotFactory(const int argc, const char* argv[]);
	~CTinyBotFactory();

	static const char * const Defaultname;
	const int m_argc;
	const char** m_argv;
	
	std::string m_Botname;
	ServerDataStringVectorPairVector m_ServerVector;
	StringPairVector * m_ChannelVector;
	StringVector m_PluginPathVector;

	CTinyBot * m_Product;

	CTinyBot * Build();
	void ParseArguments();
	void SetupServers();

	void CleanUp(ServerDataStringVectorPair * Entry);
};