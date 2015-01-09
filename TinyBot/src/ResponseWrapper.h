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

#include <string>

#include <tinyirc.h>

#include "PlugInInterface.h"

class CServer;
class CChannel;

class CResponseWrapper : public IResponseInterface
{
public:
	CResponseWrapper(tinyirc::CParser & IRCParser, CServer & ServerConnection, CChannel * Channel = NULL);
	virtual ~CResponseWrapper();

	virtual void SendMessage(const std::string & Reciever, const std::string & Message);

	void Clear();
	void Send();

private:
	tinyirc::CParser & m_IRCParser;
	CServer & m_ServerConnection;
	CChannel * m_Channel;

	std::string m_MessageBuffer;
	tinyirc::IRCMessageVector m_MessageLoop;

	void LoopMessageBack(const std::string & Message);
};

inline
void CResponseWrapper::Clear()
{
	m_MessageBuffer.assign("");
	m_MessageLoop.clear();
}