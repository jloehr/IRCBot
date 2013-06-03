#include "Channel.h"
#include "Server.h"

//------------------------------------------//
//											//
//		  Konstructor & Destructor			//
//											//
//------------------------------------------//

CChannel::CChannel(const std::string & Name, const std::string & Pass, const PluginPairVector & Plugins, CServer & ServerConnection, CResponseWrapper & ResponseWrapper)
	:m_Name(Name), m_Pass(Pass)
	,m_ServerConnection(ServerConnection), m_IRCParser(ServerConnection.GetIRCParser()), m_ResponseInterface(ResponseWrapper)
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

	m_ResponseInterface.Clear();

	for(ChannelPluginVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(*it)->OnUserList(Nicks, m_ResponseInterface);
	}

	if(m_ResponseInterface.Filled())
	{
		m_ServerConnection.Send(m_ResponseInterface.GetResponse());
	}
}

void CChannel::OnTopic(const tinyirc::IRCMessage & Message)
{
	m_ResponseInterface.Clear();

	for(ChannelPluginVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(*it)->OnTopic(*Message.Data.Topic.Topic, m_ResponseInterface);
	}

	if(m_ResponseInterface.Filled())
	{
		m_ServerConnection.Send(m_ResponseInterface.GetResponse());
	}
}

void CChannel::OnChannelMode(const tinyirc::IRCMessage & Message)
{
	m_ResponseInterface.Clear();

	for(ChannelPluginVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(*it)->OnChannelMode(*Message.Data.ChannelMode.SenderNick, *Message.Data.ChannelMode.SenderUser, *Message.Data.ChannelMode.SenderHost, Message.Data.ChannelMode.ModeAsChar,  Message.Data.ChannelMode.SetMode,  *Message.Data.ChannelMode.Param, m_ResponseInterface);
	}

	if(m_ResponseInterface.Filled())
	{
		m_ServerConnection.Send(m_ResponseInterface.GetResponse());
	}
}


void CChannel::OnJoin(const tinyirc::IRCMessage & Message)
{
	m_NickList.insert(*Message.Data.Join.Nick);

	m_ResponseInterface.Clear();

	for(ChannelPluginVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(*it)->OnJoin(*Message.Data.Join.Nick, *Message.Data.Join.User, *Message.Data.Join.Host, m_ResponseInterface);
	}
	
	if(m_ResponseInterface.Filled())
	{
		m_ServerConnection.Send(m_ResponseInterface.GetResponse());
	}
}

void CChannel::OnPart(const tinyirc::IRCMessage & Message)
{
	m_NickList.erase(*Message.Data.Join.Nick);

	m_ResponseInterface.Clear();

	for(ChannelPluginVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(*it)->OnPart(*Message.Data.Part.Nick, *Message.Data.Part.User, *Message.Data.Part.Host, *Message.Data.Part.Message, m_ResponseInterface);
	}

	if(m_ResponseInterface.Filled())
	{
		m_ServerConnection.Send(m_ResponseInterface.GetResponse());
	}
}

void CChannel::OnQuit(const tinyirc::IRCMessage & Message)
{
	m_NickList.erase(*Message.Data.Join.Nick);

	m_ResponseInterface.Clear();

	for(ChannelPluginVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(*it)->OnQuit(*Message.Data.Quit.Nick, *Message.Data.Quit.User, *Message.Data.Quit.Host, *Message.Data.Quit.Message, m_ResponseInterface);
	}

	if(m_ResponseInterface.Filled())
	{
		m_ServerConnection.Send(m_ResponseInterface.GetResponse());
	}
}

void CChannel::OnKick(const tinyirc::IRCMessage & Message)
{
	m_NickList.erase(*Message.Data.Join.Nick);

	m_ResponseInterface.Clear();

	for(ChannelPluginVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(*it)->OnKick(*Message.Data.Kick.Nick, *Message.Data.Kick.User, *Message.Data.Kick.Host, *Message.Data.Kick.Victim, *Message.Data.Kick.Reason, m_ResponseInterface);
	}

	if(m_ResponseInterface.Filled())
	{
		m_ServerConnection.Send(m_ResponseInterface.GetResponse());
	}
}

void CChannel::OnNickChange(const tinyirc::IRCMessage & Message)
{
	m_NickList.erase(*Message.Data.NickChange.Nick);
	m_NickList.insert(*Message.Data.NickChange.NewNick);

	m_ResponseInterface.Clear();

	for(ChannelPluginVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(*it)->OnNickChange(*Message.Data.NickChange.Nick, *Message.Data.NickChange.User, *Message.Data.NickChange.Host, *Message.Data.NickChange.NewNick, m_ResponseInterface);
	}

	if(m_ResponseInterface.Filled())
	{
		m_ServerConnection.Send(m_ResponseInterface.GetResponse());
	}
}

void CChannel::OnChangedTopic(const tinyirc::IRCMessage & Message)
{
	m_ResponseInterface.Clear();

	for(ChannelPluginVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(*it)->OnChangedTopic(*Message.Data.TopicChanged.Nick, *Message.Data.TopicChanged.User, *Message.Data.TopicChanged.Host, *Message.Data.TopicChanged.NewTopic, m_ResponseInterface);
	}

	if(m_ResponseInterface.Filled())
	{
		m_ServerConnection.Send(m_ResponseInterface.GetResponse());
	}
}

void CChannel::OnNotice(const tinyirc::IRCMessage & Message)
{
	m_ResponseInterface.Clear();

	for(ChannelPluginVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(*it)->OnNotice(*Message.Data.Message.Nick, *Message.Data.Message.User, *Message.Data.Message.Host, *Message.Data.Message.Message, m_ResponseInterface);
	}

	if(m_ResponseInterface.Filled())
	{
		m_ServerConnection.Send(m_ResponseInterface.GetResponse());
	}
}

void CChannel::OnMessage(const tinyirc::IRCMessage & Message)
{
	m_ResponseInterface.Clear();

	for(ChannelPluginVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(*it)->OnMessage(*Message.Data.Message.Nick, *Message.Data.Message.User, *Message.Data.Message.Host, *Message.Data.Message.Message, m_ResponseInterface);
	}

	if(m_ResponseInterface.Filled())
	{
		m_ServerConnection.Send(m_ResponseInterface.GetResponse());
	}
}
