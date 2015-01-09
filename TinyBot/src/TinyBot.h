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

	void Connect(const ServerData & Server, const StringPairVector & Channels);

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