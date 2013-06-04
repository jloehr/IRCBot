#include "Channel.h"

//------------------------------------------//
//											//
//		  Konstructor & Destructor			//
//											//
//------------------------------------------//

CChannel::CChannel(const std::string & ChannelName)
	:m_ChannelName(ChannelName)
	,m_LogEnabled(true)
{

}

CChannel::~CChannel()
{
	LogAndClearUserList();
}

//------------------------------------------//
//											//
//				   Calls					//
//											//
//------------------------------------------//

void CChannel::OnUserList(const std::vector<std::string> & UserList, IResponseInterface & Response)
{
	for(std::vector<std::string>::const_iterator it = UserList.begin(); it != UserList.end(); ++it)
	{
		m_NickList.insert(*it);
	}
}

void CChannel::OnTopic(const std::string & Topic, IResponseInterface & Response)
{

}

void CChannel::OnChannelMode(const std::string & Nick, const std::string & User, const std::string & Host, const char Mode, const bool Set, const std::string & Param, IResponseInterface & Response)
{
	if(m_LogEnabled)
	{
		std::string SetSymbol = Set ? "+" : "-";
		std::string LogMessage("mode/" + m_ChannelName + " [" + SetSymbol + Mode + " " + Param + "] by " + Nick);
		CDatabaseWrapper::Singleton.LogMessage(m_ChannelName, LogMessage);
	}
}

void CChannel::OnJoin(const std::string & Nick, const std::string & User, const std::string & Host, IResponseInterface & Response)
{
	m_NickList.insert(Nick);

	if(m_LogEnabled)
	{
		std::string LogMessage(Nick + " [" + User + "@"+ Host + "] has joined " + m_ChannelName);
		CDatabaseWrapper::Singleton.LogMessage(m_ChannelName, LogMessage);
	}
}

void CChannel::OnPart(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response)
{
	m_NickList.erase(Nick);

	if(m_LogEnabled)
	{
		std::string LogMessage(Nick + " [" + User + "@"+ Host + "] has left " + m_ChannelName + " [" + Message + "]");
		CDatabaseWrapper::Singleton.LogMessage(m_ChannelName, LogMessage);
		CDatabaseWrapper::Singleton.UserLeft(m_ChannelName, Nick, "[Part]" + Message);
	}
}

void CChannel::OnQuit(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response)
{
	m_NickList.erase(Nick);

	if(m_LogEnabled)
	{
		std::string LogMessage(Nick + " [" + User + "@"+ Host + "] has quit " + m_ChannelName + " [" + Message + "]");
		CDatabaseWrapper::Singleton.LogMessage(m_ChannelName, LogMessage);
		CDatabaseWrapper::Singleton.UserLeft(m_ChannelName, Nick, "[Quit]" + Message);
	}
}

void CChannel::OnKick(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Victim, const std::string & Reason, IResponseInterface & Response)
{
	m_NickList.erase(Victim);

	if(m_LogEnabled)
	{
		std::string LogMessage(Victim + " was kicked from " + m_ChannelName + " by " + Nick + " [" + Reason + "]");
		CDatabaseWrapper::Singleton.LogMessage(m_ChannelName, LogMessage);
		CDatabaseWrapper::Singleton.UserLeft(m_ChannelName, Victim, "[Kicked by " + Nick + "]" + Reason);
	}
}

void CChannel::OnNickChange(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & NewNick, IResponseInterface & Response)
{
	m_NickList.erase(Nick);
	m_NickList.insert(NewNick);

	if(m_LogEnabled)
	{
		std::string LogMessage(Nick + " has changed his nick to " + NewNick);
		CDatabaseWrapper::Singleton.LogMessage(m_ChannelName, LogMessage);
		CDatabaseWrapper::Singleton.UserLeft(m_ChannelName, Nick, "[NickChange] New Nick " + NewNick);
	}
}

void CChannel::OnChangedTopic(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & NewTopic, IResponseInterface & Response)
{
	if(m_LogEnabled)
	{
		std::string LogMessage(Nick + " changed the topic of " + m_ChannelName + " to: " + NewTopic);
		CDatabaseWrapper::Singleton.LogMessage(m_ChannelName, LogMessage);	
	}
}

void CChannel::OnNotice(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response)
{

}

void CChannel::OnMessage(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response)
{
	if(m_LogEnabled)
	{
		std::string LogMessage("<" + Nick + "> " + Message);
		CDatabaseWrapper::Singleton.LogMessage(m_ChannelName, LogMessage);
	}
}

void CChannel::OnShutdown(IResponseInterface & Response)
{
	LogAndClearUserList();
}

void CChannel::OnDisconnect()
{
	LogAndClearUserList();
}


//------------------------------------------//
//											//
//				 User List					//
//											//
//------------------------------------------//

void CChannel::LogAndClearUserList()
{
	LogUserList(std::string("Bot was shutdown"));
	m_NickList.clear();
}


void CChannel::LogUserList(const std::string & Reason)
{
	for(StringSet::iterator it = m_NickList.begin(); it != m_NickList.end(); ++it)
	{		
		CDatabaseWrapper::Singleton.UserLeft(m_ChannelName, (*it), Reason);		
	}

}