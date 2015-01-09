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
#include <strings.h>
#include <ev++.h>

//#include "TinyBot.h"
class CTinyBot;

class CControlPanel
{
public:
	CControlPanel(CTinyBot * Bot);
	~CControlPanel();

	void Init();
	void Start();
	void Stop();

private:
	CTinyBot & m_Bot;
	ev::io m_ev_io_StdInWatcher;

	void io_cb_StdIn(ev::io   &w, int revents);

};