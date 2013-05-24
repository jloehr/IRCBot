#include "Server.h"

const ev_tstamp	CServer::RECONNECT_INTERVALL = 60.;

//------------------------------------------//
//											//
//		  Konstructor & Destructor			//
//											//
//------------------------------------------//

CServer::CServer(const std::string * Botname, const std::string * ServerAdress, const std::string * ServerPort, StringPairVector * Channels)
	:m_Botname(*Botname), m_ServerAdress(*ServerAdress), m_ServerPort(*ServerPort)
	,m_Socketfd(-1), m_Connected(false)
{
	for(StringPairVector::iterator it = Channels->begin(); it != Channels->end(); ++it)
	{
		m_Channles.insert(std::pair<std::string, CChannel * >((*((*it)->first)), new CChannel((*it)->first, (*it)->second)));
	}

	m_ReconnectTimer.set<CServer, &CServer::timer_cb_TryReconnect>(this);
	m_SocketWatcher.set<CServer, &CServer::io_cb_SocketRead>(this);
}

CServer::~CServer()
{
	Close();

	for(ChannelMap::iterator it = m_Channles.begin(); it != m_Channles.end(); ++it)
	{
		delete it->second;
	}

	m_Channles.clear();
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
		Output::Error("Server", { "GetAddrInfo faild for ", m_ServerAdress.c_str(), ":", m_ServerPort.c_str(), " -> ", gai_strerror(result)});
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
}

void CServer::Reconnect()
{
	Output::Log(m_ServerAdress.c_str(), {"Reconnecting"});

	Close();

	if(!m_ReconnectTimer.is_active())
	{
		m_ReconnectTimer.start();
	}
}

void CServer::Disconnect()
{
	Output::Log(m_ServerAdress.c_str(), {"Disconnecting"});

	/*if(m_Connected)
	{
		//Send Disconnect
	}*/

	Close();
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

	unsigned int ReadBytes = 0;

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

	m_ReadBuffer[ReadBytes] = '\0';

	Output::Log(m_ServerAdress.c_str(), {m_ReadBuffer});

	//Parse

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

	Output::Log(m_ServerAdress.c_str(), {"Sending"});

	int result = send(m_Socketfd, Message.c_str(), Message.size(), MSG_NOSIGNAL);

	if(result < 0)
	{
		Output::Error("Server", {"Error on sending Data to ", m_ServerAdress.c_str(), ". Reconnecting!"});
		Reconnect();
		return false;
	}

	return true;
}