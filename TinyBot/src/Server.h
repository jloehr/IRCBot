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

#include <map>
#include <utility>
#include <vector>
#include <string>
#include <string.h>
#include <strings.h> 
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <ev++.h>

#include <tinyirc.h>

#include "types.h"
#include "Output.h"
#include "Channel.h"
#include "PlugInInterface.h"
#include "PlugIn.h"
#include "ResponseWrapper.h"

class CServer
{
public:
	CServer(const std::string & Botname, const std::string & ServerAdress, const std::string & ServerPort, const StringPairVector & Channels, const PluginVector & Plugins);
	~CServer();

	void Init();
	void Start();
	void Stop();

	bool Send(const std::string & Message);

	tinyirc::CParser & GetIRCParser();
	const std::string & GetBotname();

private:
	static const ev_tstamp RECONNECT_INTERVALL;
	static const size_t READ_BUFFER_SIZE = 8192;

	const std::string & m_Botname;
	const std::string m_ServerAdress;
	const std::string m_ServerPort;

	PluginPairVector m_Plugins;
	ChannelMap m_Channels; 
	tinyirc::CParser m_IRCParser;
	ev::timer m_ReconnectTimer;
	ev::io m_SocketWatcher;
	char m_ReadBuffer[READ_BUFFER_SIZE];

	int m_Socketfd;
	bool m_Connected;

	void GetPlugInInstances(const PluginVector & Plugins);
	void InstanciateChannels(const StringPairVector & Channels);

	void DeleteChannels();
	void FreePlugins();

	bool CreateSocket();
	bool Connect();
	void Close();
	void Reconnect();
	void Disconnect();
	void StartSocketWatcher();

	void Login();
	void JoinChannels();
	void ResetChannels();

	void timer_cb_TryReconnect(ev::timer &w, int revents);
	void io_cb_SocketRead(ev::io &w, int revents);

	void IssueMessages(const tinyirc::IRCMessage & Message);
	void ProcessWelcome(const tinyirc::IRCMessage & Message);
	void ProcessMOTD(const tinyirc::IRCMessage & Message);
	void ProcessUserMode(const tinyirc::IRCMessage & Message);

	void ProcessPrvtMessage(const tinyirc::IRCMessage & Message);
	void ProcessNotice(const tinyirc::IRCMessage & Message);

	void ProcessUserList(const tinyirc::IRCMessage & Message);
	void ProcessTopic(const tinyirc::IRCMessage & Message);
	void ProcessNick(const tinyirc::IRCMessage & Message);
	void ProcessJoin(const tinyirc::IRCMessage & Message);
	void ProcessPart(const tinyirc::IRCMessage & Message);
	void ProcessQuit(const tinyirc::IRCMessage & Message);
	void ProcessChannelMode(const tinyirc::IRCMessage & Message);
	void ProcessTopicChanged(const tinyirc::IRCMessage & Message);
	void ProcessKick(const tinyirc::IRCMessage & Message);
};

inline
tinyirc::CParser & CServer::GetIRCParser()
{
	return m_IRCParser;
}
inline
const std::string & CServer::GetBotname()
{
	return m_Botname;
}