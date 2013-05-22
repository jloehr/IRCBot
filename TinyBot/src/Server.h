#pragma once

#include <map>
#include <string>

#include "types.h"
#include "Channel.h"


class CServer
{
public:
	CServer(const std::string * Botname, const std::string * ServerAdress, const std::string * ServerPort, StringPairVector * Channels);
	~CServer();

private:
	const std::string & m_Botname;
	const std::string m_ServerAdress;
	const std::string m_ServerPort;

	ChannelMap m_Channles; 

};