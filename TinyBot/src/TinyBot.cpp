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
#include "TinyBot.h"


//------------------------------------------//
//											//
//		  Konstructor & Destructor			//
//											//
//------------------------------------------//

CTinyBot::CTinyBot(const std::string Botname, const StringVector & Plugins)
	:m_Botname(Botname)
	,m_ControlPanel(this)
{
	LoadPlugins(Plugins);
}

CTinyBot::~CTinyBot()
{
	for(ServerVector::iterator it = m_Servers.begin(); it != m_Servers.end(); ++it)
	{
		delete (*it);
	}
	
	m_Servers.clear();

	for(PluginVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		delete (*it);
	}

	m_Plugins.clear();
}
	

//------------------------------------------//
//											//
//				Load Plugin					//
//											//
//------------------------------------------//

void CTinyBot::LoadPlugins(const StringVector & Plugins)
{
	for(StringVector::const_iterator it = Plugins.begin(); it != Plugins.end(); ++it)
	{
		CPlugIn * Tmp = new CPlugIn(*it);

		if(Tmp->Load())
		{
				m_Plugins.push_back(Tmp);
		}
		else
		{
			delete Tmp;
		}
	}
}


//------------------------------------------//
//											//
//				Run & Stop					//
//											//
//------------------------------------------//

void CTinyBot::Run()
{
	Output::Log({"Hello my name is ", m_Botname.c_str(), "."});

	Init();

	StartWatchers();

	RunLoop();

	StopWatchers();
}

void CTinyBot::RunLoop()
{
	struct ev_loop * loop = ev_default_loop(0);

	ev_run(loop, 0);
}

void CTinyBot::Stop()
 {
	ev_break (EV_DEFAULT_ EVBREAK_ALL);

	Output::Log({"Breaking Loop..."});
 }

//------------------------------------------//
//											//
//					Init					//
//											//
//------------------------------------------//

void CTinyBot::Init()
{
	m_ControlPanel.Init();

	for(ServerVector::iterator it = m_Servers.begin(); it != m_Servers.end(); ++it)
	{
		(*it)->Init();
	}
}

//------------------------------------------//
//											//
//				  Watchers					//
//											//
//------------------------------------------//

void CTinyBot::StartWatchers()
{
	m_ControlPanel.Start();

	for(ServerVector::iterator it = m_Servers.begin(); it != m_Servers.end(); ++it)
	{
		(*it)->Start();
	}
	
}

void CTinyBot::StopWatchers()
{
	m_ControlPanel.Stop();

	for(ServerVector::iterator it = m_Servers.begin(); it != m_Servers.end(); ++it)
	{
		(*it)->Stop();
	}
}

//------------------------------------------//
//											//
//				Connect						//
//											//
//------------------------------------------//

void CTinyBot::Connect(const ServerData & Server, const StringPairVector & Channels)
{
	m_Servers.push_back(new CServer(m_Botname, *(Server.Adress), *(Server.Port), *(Server.Pass), Channels, m_Plugins));
}
