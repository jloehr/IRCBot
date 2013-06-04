#include "Server.h"

const ev_tstamp	CServer::RECONNECT_INTERVALL = 60.;

//------------------------------------------//
//											//
//		  Konstructor & Destructor			//
//											//
//------------------------------------------//

CServer::CServer(const std::string & Botname, const std::string & ServerAdress, const std::string & ServerPort, const StringPairVector & Channels, const PluginVector & Plugins)
	:m_Botname(Botname), m_ServerAdress(ServerAdress), m_ServerPort(ServerPort)
	,m_Socketfd(-1), m_Connected(false)
{
	m_ReconnectTimer.set<CServer, &CServer::timer_cb_TryReconnect>(this);
	m_SocketWatcher.set<CServer, &CServer::io_cb_SocketRead>(this);

	GetPlugInInstances(Plugins);

	InstanciateChannels(Channels);
}

CServer::~CServer()
{
	DeleteChannels();

	FreePlugins();

	Close();
}

void CServer::GetPlugInInstances(const PluginVector & Plugins)
{
	for(PluginVector::const_iterator it = Plugins.begin(); it != Plugins.end(); ++it)
	{
		IServerInterface * Tmp = (*it)->CreateInterface(m_ServerAdress, m_Botname);
		if(Tmp != NULL)
		{
			m_Plugins.push_back(PluginPair(Tmp, (*it)));
		}
	}
}

void CServer::InstanciateChannels(const StringPairVector & Channels)
{
	for(StringPairVector::const_iterator it = Channels.begin(); it != Channels.end(); ++it)
	{
		m_Channels.insert(std::pair<std::string, CChannel * >((*((*it)->first)), new CChannel((*(*it)->first), (*(*it)->second), m_Plugins, *this)));
	}

}

void CServer::DeleteChannels()
{

	for(ChannelMap::iterator it = m_Channels.begin(); it != m_Channels.end(); ++it)
	{
		delete it->second;
	}

	m_Channels.clear();
}

void CServer::FreePlugins()
{
	for(PluginPairVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(it->second)->FreeInstance(it->first);
	}

	m_Plugins.clear();
}

//------------------------------------------//
//											//
//					Init					//
//											//
//------------------------------------------//

bool CServer::CreateSocket()
{
	if(m_Socketfd != -1)
	{
		return true;
	}

	Output::Log(m_ServerAdress.c_str(), {"Creating socket"});

	m_Socketfd = socket(AF_INET, SOCK_STREAM, 0);

	if(m_Socketfd == -1)
	{
		Output::Error("Server", {"Can't create socket for ", m_ServerAdress.c_str()});
		return false;
	}

	return true;;
}

//------------------------------------------//
//											//
//					Connect					//
//											//
//------------------------------------------//

bool CServer::Connect()
{
	if(m_Connected)
	{
		return true;
	}

	if(!CreateSocket())
	{
		return false;
	}

	Output::Log(m_ServerAdress.c_str(), {"Attempt to connect"});

	int result = 0;
	struct addrinfo hints, * adresses, * tmp;

	bzero(&hints, sizeof(hints));
 	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_INET;

	result = getaddrinfo(m_ServerAdress.c_str(), m_ServerPort.c_str(), &hints, &adresses);

	if(result != 0)
	{
		int SystemErrorNum = errno;

		Output::Error("Server", { "GetAddrInfo faild for ", m_ServerAdress.c_str(), ":", m_ServerPort.c_str(), " -> ", gai_strerror(result)});
		
		if(result == EAI_SYSTEM)
		{
			Output::Error("Server", { "System Error -> ", strerror(SystemErrorNum)});
		
		}

		return false;
	}

	for(tmp = adresses; tmp != NULL; tmp = tmp->ai_next)
	{
		result = connect(m_Socketfd, tmp->ai_addr, tmp->ai_addrlen);

		if(result != -1)
		{
			break;
		}
	}

	freeaddrinfo(adresses); 

	if(tmp == NULL)
	{
		Output::Error("Server", { "Can't connect to ", m_ServerAdress.c_str(), ":", m_ServerPort.c_str()});
		return false;
	}

	Output::Log(m_ServerAdress.c_str(), {"Connected"});

	m_Connected = true;
	return true;
}


void CServer::Login()
{
	std::string PackageBuffer;

	try
	{
		m_IRCParser.Register(m_Botname, m_Botname, PackageBuffer);
	}
	catch(tinyirc::IRCException & exception)
	{
		Output::Error("Server", {"Exception cought on Login ->", exception.what()});
		Stop();
		return;
	}

	Send(PackageBuffer);
}

//------------------------------------------//
//											//
//					Close					//
//											//
//------------------------------------------//

void CServer::Close()
{
	if(m_Socketfd != -1)
	{
		return;
	}

	Output::Log(m_ServerAdress.c_str(), {"Closing"});

	m_SocketWatcher.stop();

	close(m_Socketfd);
	m_Socketfd = -1;
	m_Connected = false;

	for(PluginPairVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(it->first)->OnDisconnect();
	}
}

void CServer::Reconnect()
{
	Output::Log(m_ServerAdress.c_str(), {"Reconnecting"});

	Close();

	if(!m_ReconnectTimer.is_active())
	{
		m_ReconnectTimer.start();
		ResetChannels();
	}
}

void CServer::Disconnect()
{
	Output::Log(m_ServerAdress.c_str(), {"Disconnecting"});

	if(m_Connected)
	{
		//Send Disconnect
		CResponseWrapper ResponseInterface(m_IRCParser, *this);

		for(PluginPairVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
		{
			(it->first)->OnShutdown(ResponseInterface);
		}

		ResponseInterface.Send();

		std::string Package;

		try
		{
			std::string ExitMessage("Bot is shutting down!");
			m_IRCParser.GetClosingPackage(ExitMessage, Package);
		}
		catch(tinyirc::IRCException & exception)
		{
			Output::Error("Server", { "Exception cought on Disconnect -> ", exception.what()});
		}

		Send(Package);

		shutdown(m_Socketfd, SHUT_WR);
	}
	else
	{
		Close();
	}
}

//------------------------------------------//
//											//
//					Init					//
//											//
//------------------------------------------//

void CServer::Init()
{
	//Setup the reconnect Timer ti fire immediatly
	m_ReconnectTimer.set(0.0, RECONNECT_INTERVALL);
}

//------------------------------------------//
//											//
//				Start & Stop				//
//											//
//------------------------------------------//

void CServer::Start()
{
	Output::Log(m_ServerAdress.c_str(), {"Starting"});

	m_ReconnectTimer.start();
}

void CServer::Stop()
{
	Output::Log(m_ServerAdress.c_str(), {"Stopping"});

	Disconnect();

	m_ReconnectTimer.stop();
	m_SocketWatcher.stop();
}

void CServer::StartSocketWatcher()
{
	if(m_Connected)
	{
		m_SocketWatcher.start(m_Socketfd, ev::READ);
	}
	else
	{
		Output::Error("Server", {"Attempt to start disconnected Socket Watcher"});
	}
}

//------------------------------------------//
//											//
//				 TimerEvent					//
//											//
//------------------------------------------//


void CServer::timer_cb_TryReconnect(ev::timer &w, int revents)
{
	Output::Log(m_ServerAdress.c_str(), {"Reconnect Attempt"});

	if(revents & ev::ERROR)
	{
		Output::Error("Server", {"Unspecified Error signaled by Libev in the reconnect timer for ", m_ServerAdress.c_str() , ". Stopping the Server reconnect attempt."});
		Stop();
	}

	if(Connect())
	{
		m_ReconnectTimer.stop();
		StartSocketWatcher();
		Login();
	}
}

//------------------------------------------//
//											//
//			 Socket Callback				//
//											//
//------------------------------------------//

void CServer::io_cb_SocketRead(ev::io &w, int revents)
{	
	if(revents & ev::ERROR)
	{
		Output::Error("Server", {"Unspecified Error signaled by Libev in the SocketWatcher for ", m_ServerAdress.c_str() , ". Stopping the Server reconnect attempt."});
		Stop();
		return;
	}

	Output::Log(m_ServerAdress.c_str(), {"Incomming"});

	ssize_t ReadBytes = 0;

	// Read read read
	ReadBytes = recv(m_Socketfd, m_ReadBuffer, CServer::READ_BUFFER_SIZE, 0);

	if(ReadBytes < 0)
	{
		Output::Error("Server", {"Error while reading from ", m_ServerAdress.c_str() , ". Reconnecting!"});
		Reconnect();
		return;
	}

	if(ReadBytes == 0)
	{
		//Closed
		Reconnect();
		return;
	}

	bool CloseConnection = false;
	std::string Recieved(m_ReadBuffer, (size_t)ReadBytes);
	std::string Response;
	tinyirc::IRCMessageVector Messages;

	Output::Log(m_ServerAdress.c_str(), {Recieved.c_str()});

	//Parse
	try 
	{
		CloseConnection = m_IRCParser.Parse(Recieved, Response, Messages);
	}
	catch(tinyirc::IRCException & exception)
	{
		Output::Error("Server", {"Exception cought while Parsing Incomming -> ",exception.what()});
		Reconnect();
		return;
	}
	
	if(!Response.empty())
	{
		Send(Response);
	}

	if(CloseConnection)
	{
		Reconnect();
		return;
	}

	//Issue Messages
	for(tinyirc::IRCMessageVector::iterator it = Messages.begin(); it != Messages.end(); ++it)
	{
		IssueMessages(*it);
	}

}

//------------------------------------------//
//											//
//			  Issue Messages				//
//											//
//------------------------------------------//

	
void CServer::IssueMessages(const tinyirc::IRCMessage & Message)
{
	switch(Message.Type)
	{
		case tinyirc::IRCMessageType::Welcome:
			ProcessWelcome(Message);
			break;
		case tinyirc::IRCMessageType::MOTD:
			ProcessMOTD(Message);
			break;
		case tinyirc::IRCMessageType::UserMode:
			ProcessUserMode(Message);
			break;

		case tinyirc::IRCMessageType::Message:
			ProcessPrvtMessage(Message);
			break;
		case tinyirc::IRCMessageType::Notice:
			ProcessNotice(Message);
			break;


		case tinyirc::IRCMessageType::UserList:
			ProcessUserList(Message);
			break;
		case tinyirc::IRCMessageType::Topic:
			ProcessTopic(Message);
			break;
		case tinyirc::IRCMessageType::NickChange:
			ProcessNick(Message);
			break;
		case tinyirc::IRCMessageType::Join:
			ProcessJoin(Message);
			break;
		case tinyirc::IRCMessageType::Part:
			ProcessPart(Message);
			break;
		case tinyirc::IRCMessageType::Quit:
			ProcessQuit(Message);
			break;
		case tinyirc::IRCMessageType::ChannelMode:
			ProcessChannelMode(Message);
			break;
		case tinyirc::IRCMessageType::TopicChanged:
			ProcessTopicChanged(Message);
			break;
		case tinyirc::IRCMessageType::Kick:
			ProcessKick(Message);
			break;

	}
}

void CServer::ProcessWelcome(const tinyirc::IRCMessage & Message)
{
	JoinChannels();

	CResponseWrapper ResponseInterface(m_IRCParser, *this);

	for(PluginPairVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(it->first)->OnWelcome(*Message.Data.Welcome.Message, ResponseInterface);
	}
	
	ResponseInterface.Send();
}

void CServer::ProcessMOTD(const tinyirc::IRCMessage & Message)
{
	CResponseWrapper ResponseInterface(m_IRCParser, *this);

	for(PluginPairVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(it->first)->OnMOTD(*Message.Data.MOTD.Message, ResponseInterface);
	}
	
	ResponseInterface.Send();
}

void CServer::ProcessUserMode(const tinyirc::IRCMessage & Message)
{
	CResponseWrapper ResponseInterface(m_IRCParser, *this);

	for(PluginPairVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
	{
		(it->first)->OnUserMode(*Message.Data.UserMode.SenderNick, *Message.Data.UserMode.SenderUser, *Message.Data.UserMode.SenderHost, Message.Data.UserMode.ModeAsChar,  Message.Data.UserMode.SetMode, ResponseInterface);
	}
	
	ResponseInterface.Send();
}

void CServer::ProcessPrvtMessage(const tinyirc::IRCMessage & Message)
{
	ChannelMap::iterator Result = m_Channels.find(*Message.Data.Message.Reciever);

	if(Result == m_Channels.end())
	{
		CResponseWrapper ResponseInterface(m_IRCParser, *this);

		for(PluginPairVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
		{
			(it->first)->OnPrvtMessage(*Message.Data.Message.Nick, *Message.Data.Message.User, *Message.Data.Message.Host, *Message.Data.Message.Message, ResponseInterface);
		}

		ResponseInterface.Send();
	}
	else
	{
		Result->second->OnMessage(Message);
	}

}

void CServer::ProcessNotice(const tinyirc::IRCMessage & Message)
{
	ChannelMap::iterator Result = m_Channels.find(*Message.Data.Message.Reciever);

	if(Result == m_Channels.end())
	{
		CResponseWrapper ResponseInterface(m_IRCParser, *this);

		for(PluginPairVector::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
		{
			(it->first)->OnNotice(*Message.Data.Message.Nick, *Message.Data.Message.User, *Message.Data.Message.Host, *Message.Data.Message.Message, ResponseInterface);
		}

		ResponseInterface.Send();
	}
	else
	{
		Result->second->OnNotice(Message);
	}
}

void CServer::ProcessUserList(const tinyirc::IRCMessage & Message)
{
	ChannelMap::iterator Result = m_Channels.find(*Message.Data.UserList.Channel);

	if(Result != m_Channels.end())
	{
		Result->second->OnUserList(Message);
	}
}

void CServer::ProcessTopic(const tinyirc::IRCMessage & Message)
{
	ChannelMap::iterator Result = m_Channels.find(*Message.Data.Topic.Channel);

	if(Result != m_Channels.end())
	{
		Result->second->OnTopic(Message);
	}
}

void CServer::ProcessNick(const tinyirc::IRCMessage & Message)
{
	for(ChannelMap::iterator it = m_Channels.begin(); it != m_Channels.end(); ++it)
	{
		if(it->second->UserIsIn(*Message.Data.NickChange.Nick))
		{
			it->second->OnNickChange(Message);
		}
	}
}

void CServer::ProcessJoin(const tinyirc::IRCMessage & Message)
{
	ChannelMap::iterator Result = m_Channels.find(*Message.Data.Join.Channel);

	if(Result != m_Channels.end())
	{
		Result->second->OnJoin(Message);
	}
}

void CServer::ProcessPart(const tinyirc::IRCMessage & Message)
{
	ChannelMap::iterator Result = m_Channels.find(*Message.Data.Part.Channel);

	if(Result != m_Channels.end())
	{
		Result->second->OnPart(Message);
	}
}

void CServer::ProcessQuit(const tinyirc::IRCMessage & Message)
{	
	for(ChannelMap::iterator it = m_Channels.begin(); it != m_Channels.end(); ++it)
	{
		if(it->second->UserIsIn(*Message.Data.Quit.Nick))
		{
			it->second->OnQuit(Message);
		}
	}
}

void CServer::ProcessChannelMode(const tinyirc::IRCMessage & Message)
{
	ChannelMap::iterator Result = m_Channels.find(*Message.Data.ChannelMode.Channel);

	if(Result != m_Channels.end())
	{
		Result->second->OnChannelMode(Message);
	}
}

void CServer::ProcessTopicChanged(const tinyirc::IRCMessage & Message)
{
	ChannelMap::iterator Result = m_Channels.find(*Message.Data.TopicChanged.Channel);

	if(Result != m_Channels.end())
	{
		Result->second->OnChangedTopic(Message);
	}
}

void CServer::ProcessKick(const tinyirc::IRCMessage & Message)
{
	ChannelMap::iterator Result = m_Channels.find(*Message.Data.Kick.Channel);

	if(Result != m_Channels.end())
	{
		Result->second->OnKick(Message);
	}
}



//------------------------------------------//
//											//
//				  Channels					//
//											//
//------------------------------------------//

void CServer::JoinChannels()
{
	for(ChannelMap::iterator it = m_Channels.begin(); it != m_Channels.end(); ++it)
	{
		it->second->Join();
	}
}

void CServer::ResetChannels()
{
	for(ChannelMap::iterator it = m_Channels.begin(); it != m_Channels.end(); ++it)
	{
		it->second->Reset();
	}
}

//------------------------------------------//
//											//
//				 Socket Send				//
//											//
//------------------------------------------//

bool CServer::Send(const std::string & Message)
{
	if(!m_Connected)
	{
		Output::Error("Server", {"Attempt to send through unconnected Server ", m_ServerAdress.c_str()});
		return false;
	}

	if(Message.size() == 0)
	{
		Output::Error("Server", {"Attempt to send emtpy String to ", m_ServerAdress.c_str()});
		return false;	
	}

	Output::Log(m_ServerAdress.c_str(), {"Sending -> ", Message.c_str()});

	const size_t BytesSendAtOnce = 512;
	size_t BytesSend = 0;

	while(BytesSend < Message.size())
	{
		size_t BytesToSend = ((BytesSend + BytesSendAtOnce) > Message.size()) ? (Message.size() - BytesSend) : BytesSendAtOnce;
		ssize_t result = send(m_Socketfd, (Message.c_str() + BytesSend), BytesToSend, MSG_NOSIGNAL);

		if(result < 0)
		{
			Output::Error("Server", {"Error on sending Data to ", m_ServerAdress.c_str(), ". Reconnecting!"});
			Reconnect();
			return false;
		}	

		BytesSend += BytesToSend;
	}


	return true;
}