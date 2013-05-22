#include "Server.h"



//------------------------------------------//
//											//
//		  Konstructor & Destructor			//
//											//
//------------------------------------------//

CServer::CServer(const std::string * Botname, const std::string * ServerAdress, const std::string * ServerPort, StringPairVector * Channels)
	:m_Botname(*Botname), m_ServerAdress(*ServerAdress), m_ServerPort(*ServerPort)
{
	for(StringPairVector::iterator it = Channels->begin(); it != Channels->end(); ++it)
	{
		m_Channles.insert(std::pair<std::string, CChannel * >((*((*it)->first)), new CChannel((*it)->first, (*it)->second)));
	}
}

CServer::~CServer()
{
	for(ChannelMap::iterator it = m_Channles.begin(); it != m_Channles.end(); ++it)
	{
		delete it->second;
	}

	m_Channles.clear();
}