#include "Server.h"

//------------------------------------------//
//											//
//		  Konstructor & Destructor			//
//											//
//------------------------------------------//

CServer::CServer(const std::string & ServerAdress, const std::string & Botname)
	:m_Nick(Botname), m_ServerAdress(ServerAdress)
{

}

CServer::~CServer()
{
	for(ChannelSet::iterator it = m_Channels.begin(); it != m_Channels.end(); ++it)
	{
		delete (*it);
	}

	m_Channels.clear();
}

//------------------------------------------//
//											//
//		 	 	 Get Channel				//
//											//
//------------------------------------------//

IChannelInterface & CServer::GetChannelInterface(const std::string & ChannelName)
{
	CChannel * Tmp = new CChannel(ChannelName);

	typedef std::pair<ChannelSet::iterator, bool> InsertResult;
	InsertResult Result = m_Channels.insert(Tmp);

	return *Tmp;
}

//------------------------------------------//
//											//
//		 	 	 Get Channel				//
//											//
//------------------------------------------//

void CServer::OnWelcome(const std::string & WelcomeMesage, IResponseInterface & Response)
{

}

void CServer::OnMOTD(const std::string & MessageOfTheDay, IResponseInterface & Response)
{

}

void CServer::OnPrvtMessage(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response)
{

}

void CServer::OnNotice(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response)
{

}

void CServer::OnUserMode(const std::string & Nick, const std::string & User, const std::string & Host, const char Mode, const bool Set, IResponseInterface & Response)
{

}


void CServer::OnShutdown(IResponseInterface & Response)
{
	for(ChannelSet::iterator it = m_Channels.begin(); it != m_Channels.end(); ++it)
	{
		(*it)->OnShutdown(Response);
	}
}

void CServer::OnDisconnect()
{

}
