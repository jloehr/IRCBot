#include "Channel.h"

//------------------------------------------//
//											//
//		  Konstructor & Destructor			//
//											//
//------------------------------------------//

CChannel::CChannel(const std::string & ChannelName)
	:m_ChannelName(ChannelName)
{

}

CChannel::~CChannel()
{
	
}

//------------------------------------------//
//											//
//				   Calls					//
//											//
//------------------------------------------//

void CChannel::OnUserList(const std::vector<std::string> & UserList, IResponseInterface & Response)
{
}

void CChannel::OnTopic(const std::string & Topic, IResponseInterface & Response)
{

}

void CChannel::OnChannelMode(const std::string & Nick, const std::string & User, const std::string & Host, const char Mode, const bool Set, const std::string & Param, IResponseInterface & Response)
{

}

void CChannel::OnJoin(const std::string & Nick, const std::string & User, const std::string & Host, IResponseInterface & Response)
{

}

void CChannel::OnPart(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response)
{

}

void CChannel::OnQuit(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response)
{

}

void CChannel::OnKick(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Victim, const std::string & Reason, IResponseInterface & Response)
{

}

void CChannel::OnNickChange(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & NewNick, IResponseInterface & Response)
{

}

void CChannel::OnChangedTopic(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & NewTopic, IResponseInterface & Response)
{

}

void CChannel::OnNotice(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response)
{

}

void CChannel::OnMessage(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response)
{

}

void CChannel::OnShutdown(IResponseInterface & Response)
{
	
}
