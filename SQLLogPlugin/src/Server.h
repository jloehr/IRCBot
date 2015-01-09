/*
 *
 * Copyright (C) 2013 Julian Löhr
 * All rights reserved.
 *
 * This file is part of SQLLogPlugin, a Logging Plugin for TinyBot.
 * SQLLogPlugin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * SQLLogPlugin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with SQLLogPlugin.  If not, see <http://www.gnu.org/licenses/>.
 *
*/
#pragma once

#include <set>

#include "Channel.h"

typedef std::set<CChannel *> ChannelSet;

class CServer : public IServerInterface
{
public:
	CServer(const std::string & ServerAdress, const std::string & Botname);
	virtual ~CServer();

	virtual IChannelInterface & GetChannelInterface(const std::string & ChannelName);

	//Server Events
	virtual void OnWelcome		(const std::string & WelcomeMesage, IResponseInterface & Response);
	virtual void OnMOTD			(const std::string & MessageOfTheDay, IResponseInterface & Response);
	virtual void OnPrvtMessage	(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response);
	virtual void OnNotice		(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response);
	virtual void OnUserMode		(const std::string & Nick, const std::string & User, const std::string & Host, const char Mode, const bool Set, IResponseInterface & Response);

	virtual void OnShutdown		(IResponseInterface & Response);
	virtual void OnDisconnect	();

private:
	const std::string & m_Nick;
	const std::string & m_ServerAdress;

	ChannelSet m_Channels;

};