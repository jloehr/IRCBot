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