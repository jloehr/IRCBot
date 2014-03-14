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