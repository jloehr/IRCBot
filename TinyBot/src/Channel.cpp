#include "Channel.h"
#include "Server.h"

//------------------------------------------//
//											//
//		  Konstructor & Destructor			//
//											//
//------------------------------------------//

CChannel::CChannel(const std::string & Name, const std::string & Pass, const PluginPairVector & Plugins, CServer & ServerConnection)
	:m_Name(Name), m_Pass(Pass)
	,m_ServerConnection(ServerConnection), m_IRCParser(ServerConnection.GetIRCParser())
{
	for(PluginPairVector::const_iterator it = Plugins.begin(); it != Plugins.end(); ++it)
	{
		m_Plugins.push_back(&(it->first->GetChannelInterface(m_Name)));
	}
}

CChannel::~CChannel()
{
	m_Plugins.clear();
	m_NickList.clear();
}


void CChannel::Reset()
{
	m_NickList.clear();
}

//------------------------------------------//
//											//
//		  		UserIsIn					//
//											//
//------------------------------------------//

bool CChannel::UserIsIn(const std::string & Nick)
{
	return (m_NickList.find(Nick) != m_NickList.end());
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


//------------------------------------------//
//											//
//		  			Events					//
//											//
//------------------------------------------//


void CChannel::OnUserList(const tinyirc::IRCMessage & Message)
{

	StringVector & Nicks = (*Message.Data.UserList.Nicks);

	m_NickList = StringSet(Nicks.begin(), Nicks.end());

	std::string Response;

	for(ChannelPluginVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(*it)->OnUserList(Nicks, Response);
	}

	m_ServerConnection.Send(Response);
}

void CChannel::OnTopic(const tinyirc::IRCMessage & Message)
{
	std::string Response;

	for(ChannelPluginVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(*it)->OnTopic(*Message.Data.Topic.Topic, Response);
	}

	m_ServerConnection.Send(Response);
}

void CChannel::OnChannelMode(const tinyirc::IRCMessage & Message)
{
	std::string Response;

	for(ChannelPluginVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(*it)->OnChannelMode(*Message.Data.ChannelMode.SenderNick, *Message.Data.ChannelMode.SenderUser, *Message.Data.ChannelMode.SenderHost, Message.Data.ChannelMode.ModeAsChar,  Message.Data.ChannelMode.SetMode,  *Message.Data.ChannelMode.Param, Response);
	}

	m_ServerConnection.Send(Response);
}


void CChannel::OnJoin(const tinyirc::IRCMessage & Message)
{
	m_NickList.insert(*Message.Data.Join.Nick);

	std::string Response;

	for(ChannelPluginVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(*it)->OnJoin(*Message.Data.Join.Nick, *Message.Data.Join.User, *Message.Data.Join.Host, Response);
	}

	m_ServerConnection.Send(Response);

}

void CChannel::OnPart(const tinyirc::IRCMessage & Message)
{
	m_NickList.erase(*Message.Data.Join.Nick);

	std::string Response;

	for(ChannelPluginVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(*it)->OnPart(*Message.Data.Part.Nick, *Message.Data.Part.User, *Message.Data.Part.Host, *Message.Data.Part.Message, Response);
	}

	m_ServerConnection.Send(Response);
}

void CChannel::OnQuit(const tinyirc::IRCMessage & Message)
{
	m_NickList.erase(*Message.Data.Join.Nick);

	std::string Response;

	for(ChannelPluginVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(*it)->OnQuit(*Message.Data.Quit.Nick, *Message.Data.Quit.User, *Message.Data.Quit.Host, *Message.Data.Quit.Message, Response);
	}

	m_ServerConnection.Send(Response);
}

void CChannel::OnKick(const tinyirc::IRCMessage & Message)
{
	m_NickList.erase(*Message.Data.Join.Nick);

	std::string Response;

	for(ChannelPluginVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(*it)->OnKick(*Message.Data.Kick.Nick, *Message.Data.Kick.User, *Message.Data.Kick.Host, *Message.Data.Kick.Victim, *Message.Data.Kick.Reason, Response);
	}

	m_ServerConnection.Send(Response);
}

void CChannel::OnNickChange(const tinyirc::IRCMessage & Message)
{
	m_NickList.erase(*Message.Data.NickChange.Nick);
	m_NickList.insert(*Message.Data.NickChange.NewNick);

	std::string Response;

	for(ChannelPluginVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(*it)->OnNickChange(*Message.Data.NickChange.Nick, *Message.Data.NickChange.User, *Message.Data.NickChange.Host, *Message.Data.NickChange.NewNick, Response);
	}

	m_ServerConnection.Send(Response);
}

void CChannel::OnChangedTopic(const tinyirc::IRCMessage & Message)
{
	std::string Response;

	for(ChannelPluginVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(*it)->OnChangedTopic(*Message.Data.TopicChanged.Nick, *Message.Data.TopicChanged.User, *Message.Data.TopicChanged.Host, *Message.Data.TopicChanged.NewTopic, Response);
	}

	m_ServerConnection.Send(Response);
}

void CChannel::OnNotice(const tinyirc::IRCMessage & Message)
{
	std::string Response;

	for(ChannelPluginVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(*it)->OnNotice(*Message.Data.Message.Nick, *Message.Data.Message.User, *Message.Data.Message.Host, *Message.Data.Message.Message, Response);
	}

	m_ServerConnection.Send(Response);
}

void CChannel::OnMessage(const tinyirc::IRCMessage & Message)
{
	std::string Response;

	for(ChannelPluginVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(*it)->OnMessage(*Message.Data.Message.Nick, *Message.Data.Message.User, *Message.Data.Message.Host, *Message.Data.Message.Message, Response);
	}

	m_ServerConnection.Send(Response);
}
