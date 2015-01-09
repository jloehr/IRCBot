/*
 *
 * Copyright (C) 2013 Julian Löhr
 * All rights reserved.
 *
 * This file is part of TinyBot, an IRC Bot.
 * TinyBot is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * TinyBot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with TinyBot.  If not, see <http://www.gnu.org/licenses/>.
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