#include "Channel.h"
#include "Server.h"

//------------------------------------------//
//											//
//		  Konstructor & Destructor			//
//											//
//------------------------------------------//

CChannel::CChannel(const std::string * Name, const std::string * Pass, CServer * ServerConnection)
	:m_Name(*Name), m_Pass(*Pass)
	,m_ServerConnection(*ServerConnection), m_IRCParser(ServerConnection->GetIRCParser())
{

}

CChannel::~CChannel()
{

}


void CChannel::Reset()
{

}

//------------------------------------------//
//											//
//		  			Join					//
//											//
//------------------------------------------//

void CChannel::Join()
{
	std::string Package;

	try
	{
		m_IRCParser.JoinChannel(m_Name, m_Pass, Package);
	}
	catch(tinyirc::IRCException & expetion)
	{
		Output::Error("Channel", { m_Name.c_str(), " cought IRC Exception -> ", expetion.what()});
		return;
	}

	if(!Package.empty())
	{
		m_ServerConnection.Send(Package);
	}
}