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
#include <set>
#include <map>
#include <vector>
#include <stdexcept>

#include "PlugInInterface.h"
#include "DatabaseWrapper.h"

class CChannel: public IChannelInterface
{
public:
	CChannel(const std::string & BotName, const std::string & ChannelName);
	virtual ~CChannel();

	//Channel Events
	virtual void OnUserList(const std::vector<std::string> & UserList, IResponseInterface & Response);
	virtual void OnTopic(const std::string & Topic, IResponseInterface & Response);
	virtual void OnChannelMode(const std::string & Nick, const std::string & User, const std::string & Host, const char Mode, const bool Set, const std::string & Param, IResponseInterface & Response);

	virtual void OnJoin(const std::string & Nick, const std::string & User, const std::string & Host, IResponseInterface & Response);
	virtual void OnPart(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response);
	virtual void OnQuit(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response);
	virtual void OnKick(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Victim, const std::string & Reason, IResponseInterface & Response);
	virtual void OnNickChange(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & NewNick, IResponseInterface & Response);
	virtual void OnChangedTopic(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & NewTopic, IResponseInterface & Response);

	virtual void OnNotice(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response);
	virtual void OnMessage(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response);

	void OnShutdown(IResponseInterface & Response);
	void OnDisconnect();

private:
	typedef std::set<std::string> StringSet;
	typedef std::vector<std::string> StringVector;
	typedef void (CChannel::*CommandFunctionName)(const std::string & Sender, const StringVector & Parameter, IResponseInterface & Response);
	typedef std::map<std::string, CommandFunctionName> CommandFunctionMap;

	static const std::string HelpCommand;
	static const std::string LogCommand;
	static const std::string PrintLogCommand;
	static const std::string PrintLogPrvtCommand;
	static const std::string LastSeenCommand;

	static const std::string LogOutputPrefix;

	static const CommandFunctionMap m_CommandFunctionMap;

	const std::string & m_BotName;
	const std::string & m_ChannelName;
	const std::string m_CommandPrefix;

	StringSet m_NickList;
	StringSet m_Operators;
	StringSet m_Voiced;
	bool m_LogEnabled;


	void LogAndClearUserList();
	void LogUserList(const std::string & Reason);

	void ParseCommand(const std::string & MessageTail, std::string & Command, StringVector & Parameter);

	void CommandLog(const std::string & Sender, const StringVector & Parameter, IResponseInterface & Response);	
	void CommandPrint(const std::string & Sender, const StringVector & Parameter, IResponseInterface & Response);
	void CommandPrintPrvt(const std::string & Sender, const StringVector & Parameter, IResponseInterface & Response);
	void CommandLastSeen(const std::string & Sender, const StringVector & Parameter, IResponseInterface & Response);
	void CommandHelp(const std::string & Sender, const StringVector & Parameter, IResponseInterface & Response);

	void PrintCommands(IResponseInterface & Response);

	bool SendLog(const std::string & Reciever, const StringVector & Parameter, const std::string & Prefix, const std::string & Command, IResponseInterface & Response);
};
