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
#include "ControlPanel.h"
#include "TinyBot.h"

//------------------------------------------//
//											//
//		  Konstructor & Destructor			//
//											//
//------------------------------------------//

CControlPanel::CControlPanel(CTinyBot * Bot)
	:m_Bot(*Bot)
{
	m_ev_io_StdInWatcher.set<CControlPanel, &CControlPanel::io_cb_StdIn> (this);

}

CControlPanel::~CControlPanel()
{
	m_ev_io_StdInWatcher.stop();
}

//------------------------------------------//
//											//
//			Init, Start & Stop				//
//											//
//------------------------------------------//

void CControlPanel::Init()
{
	m_ev_io_StdInWatcher.set(STDIN_FILENO ,ev::READ);
}

void CControlPanel::Start()
{
	m_ev_io_StdInWatcher.start();
}

void CControlPanel::Stop()
{
	m_ev_io_StdInWatcher.stop();
}

//------------------------------------------//
//											//
//				Callback					//
//											//
//------------------------------------------//
	
void CControlPanel::io_cb_StdIn(ev::io   &w, int revents)
{

	if(revents & ev::ERROR)
	{
		Output::Error("ControlPanel", {"Unspecified Error signaled by Libev. Shutting down!"});
		m_Bot.Stop();
		return;
	}

	std::string InputBuffer;

	std::getline (std::cin, InputBuffer);

	if((InputBuffer.compare("exit") == 0) || (InputBuffer.compare("quit") == 0))
	{
		Output::Log({"Exiting..."});

		m_Bot.Stop();
	}
}