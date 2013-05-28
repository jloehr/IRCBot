#pragma once

#include <map>
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

class CServer
{
public:
	CServer(const std::string * Botname, const std::string * ServerAdress, const std::string * ServerPort, StringPairVector * Channels);
	~CServer();

	void Init();
	void Start();
	void Stop();

	bool Send(const std::string & Message);

	tinyirc::CParser & GetIRCParser();

private:
	static const ev_tstamp RECONNECT_INTERVALL;
	static const size_t READ_BUFFER_SIZE = 8192;

	const std::string & m_Botname;
	const std::string m_ServerAdress;
	const std::string m_ServerPort;

	ChannelMap m_Channles; 
	tinyirc::CParser m_IRCParser;
	ev::timer m_ReconnectTimer;
	ev::io m_SocketWatcher;
	char m_ReadBuffer[READ_BUFFER_SIZE];

	int m_Socketfd;
	bool m_Connected;


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
};

inline
tinyirc::CParser & CServer::GetIRCParser()
{
	return m_IRCParser;
}