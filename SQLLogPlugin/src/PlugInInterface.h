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

#include <string>
#include <vector>

#define PLUGIN_INTERFACE_VERSION 1010
//#define PLUGIN_VERSION_FUNCTION "GetPluginInterfaceVersion"
//#define PLUGIN_CREATE_INTERFACE_FUNCTION "CreateServerInterface"
//#define PLUGIN_FREE_INTERFACE_FUNCTION "FreeServerInterface"

class IChannelInterface;
class IServerInterface;
class IResponseInterface;

//typedef int (*PluginIntefaceVersionFunction)();
//typedef IServerInterface * (*CreateServerInterfaceFunction) (const std::string & ServerAdress, const std::string & Botname);
//typedef void (*FreeServerInterfaceFunction)(IServerInterface * Instance);


class IServerInterface
{
public:
	virtual ~IServerInterface() {};

	virtual IChannelInterface & GetChannelInterface(const std::string & ChannelName) = 0;

	//Server Events
	virtual void OnWelcome		(const std::string & WelcomeMesage, IResponseInterface & Response) = 0;
	virtual void OnMOTD			(const std::string & MessageOfTheDay, IResponseInterface & Response) = 0;
	virtual void OnPrvtMessage	(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response) = 0;
	virtual void OnNotice		(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response) = 0;
	virtual void OnUserMode		(const std::string & Nick, const std::string & User, const std::string & Host, const char Mode, const bool Set, IResponseInterface & Response) = 0;

	virtual void OnShutdown		(IResponseInterface & Response) = 0;
	virtual void OnDisconnect	() = 0;

};

class IChannelInterface
{
public:
	virtual ~IChannelInterface() {};

	//Channel Events
	virtual void OnUserList	(const std::vector<std::string> & UserList, IResponseInterface & Response) = 0;
	virtual void OnTopic	(const std::string & Topic, IResponseInterface & Response) = 0;
	virtual void OnChannelMode	(const std::string & Nick, const std::string & User, const std::string & Host, const char Mode, const bool Set, const std::string & Param, IResponseInterface & Response) = 0;

	virtual void OnJoin		(const std::string & Nick, const std::string & User, const std::string & Host, IResponseInterface & Response) = 0;
	virtual void OnPart		(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response) = 0;
	virtual void OnQuit		(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response) = 0;
	virtual void OnKick		(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Victim, const std::string & Reason, IResponseInterface & Response) = 0;
	virtual void OnNickChange	(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & NewNick, IResponseInterface & Response) = 0;
	virtual void OnChangedTopic	(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & NewTopic, IResponseInterface & Response) = 0;

	virtual void OnNotice	(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response) = 0;
	virtual void OnMessage	(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response) = 0;
};

class IResponseInterface
{
public:
	virtual ~IResponseInterface() {};

	virtual void SendMessage(const std::string & Reciever, const std::string & Message) = 0;
};