#include "Parser.h"

namespace tinyirc
{
	const size_t CParser::MaxIRCMessageLength = 512;
	const size_t CParser::MaxNickLength = 9;
	const size_t CParser::MaxChannelNameLength = 200;
	const std::string CParser::AllowedCharsForNick = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789{}[]\\|^-_`";
	const std::string CParser::NotAllowedCharsForChannel = "\0\a\r\n ,";
	const std::string CParser::NotAllowedCharsForMessages = "\0\r\n";
	const std::string CParser::CommandEnd = "\r\n";

	const CParser::ProcessFunctionMap CParser::m_ProcessFunctionMap = {
														{"001", &CParser::ProcessWelcome}, 
														{"RPL_WELCOME", &CParser::ProcessWelcome},

														{"375", &CParser::ProcessMOTD}, 
														{"RPL_MOTDSTART", &CParser::ProcessMOTD},
														{"372", &CParser::ProcessMOTD}, 
														{"RPL_MOTD", &CParser::ProcessMOTD},
														{"376", &CParser::ProcessMOTDEnd}, 
														{"RPL_ENDOFMOTD", &CParser::ProcessMOTDEnd},

														{"PING", &CParser::ProcessPing},

														{"NICK", &CParser::ProcessNick},

														{"JOIN", &CParser::ProcessJoin},
														{"PART", &CParser::ProcessPart},
														{"QUIT", &CParser::ProcessQuit},

														{"PRIVMSG", &CParser::ProcessMessage}, 
														{"NOTICE", &CParser::ProcessNotice},

														{"MODE", &CParser::ProcessMode}, 
														{"TOPIC", &CParser::ProcessTopic}, 
														{"KICK", &CParser::ProcessKick}
													};

	//------------------------------------------//
	//											//
	//		  Konstructor & Destructor			//
	//											//
	//------------------------------------------//

	CParser::CParser()
		:m_CloseConnection(false)
	{

	}

	CParser::~CParser()
	{

	}

	void CParser::Reset()
	{
		m_CloseConnection = false;
		m_RecieveBuffer.assign("");
	}

	//------------------------------------------//
	//											//
	//				   Parse 					//
	//											//
	//------------------------------------------//

	bool CParser::Parse(const std::string & Recieved, std::string & Response, IRCMessageVector & Messages)
	{
		m_RecieveBuffer += Recieved;
		m_ResponseBuffer.assign("");
		m_MessageBuffer.clear();

		size_t EndPos;

		while((EndPos = m_RecieveBuffer.find(CParser::CommandEnd)) != std::string::npos)
		{
			m_ParsingBuffer = m_RecieveBuffer.substr(0, EndPos);
			m_ParsingPosition = 0;
			m_RecieveBuffer.erase(0, EndPos + CParser::CommandEnd.size());

			ParseMessage();
			ProcessParsedMessage();
		}

		Response = m_ResponseBuffer;
		Messages = m_MessageBuffer;

		return m_CloseConnection;
	}


	void CParser::ParseMessage()
	{
		m_PrefixNick.assign("");
		m_PrefixUser.assign("");
		m_PrefixHost.assign("");
		m_Command.assign("");
		m_Params.clear();

		ParsePrefix();
		ParseCommand();
		ParseParams();
	}

	void CParser::ParsePrefix()
	{
		if(m_ParsingBuffer[m_ParsingPosition] == ':')
		{
			++m_ParsingPosition;

			size_t SpacePosition = m_ParsingBuffer.find(' ', m_ParsingPosition);

			if(SpacePosition != std::string::npos)
			{
				size_t NickStart = m_ParsingPosition;
				size_t NickLength = SpacePosition - m_ParsingPosition;
				size_t UserStart = 0;
				size_t UserLength = 0;
				size_t HostStart = 0;
				size_t HostLength = 0;

				//Check for User
				size_t NickDelimPosition = m_ParsingBuffer.find('!', m_ParsingPosition);
				if(NickDelimPosition < SpacePosition)
				{
					NickLength = NickDelimPosition - NickStart;
					UserStart = NickDelimPosition + 1;
					UserLength = SpacePosition - UserStart;

					size_t UserDelimPosition = m_ParsingBuffer.find('@', UserStart);
					if(UserDelimPosition < SpacePosition)
					{
						UserLength = UserDelimPosition - UserStart;
						HostStart = UserDelimPosition + 1;
						HostLength = SpacePosition - HostStart;
					}

				}

				m_PrefixNick = m_ParsingBuffer.substr(NickStart, NickLength);
				if(UserLength > 0)
				{
					m_PrefixUser = m_ParsingBuffer.substr(UserStart, UserLength);
					if(HostLength > 0)
					{
						m_PrefixHost = m_ParsingBuffer.substr(HostStart, HostLength);
					}
				}

				m_ParsingPosition = SpacePosition + 1;
			}
		}
	}

	void CParser::ParseCommand()
	{
		size_t SpacePosition = m_ParsingBuffer.find(' ', m_ParsingPosition);

		if(SpacePosition != std::string::npos)
		{
			m_Command = m_ParsingBuffer.substr(m_ParsingPosition, SpacePosition - m_ParsingPosition);
			m_ParsingPosition = SpacePosition + 1;
		}
	}
	
	void CParser::ParseParams()
	{
		while(m_ParsingPosition < m_ParsingBuffer.size())
		{
			if(m_ParsingBuffer[m_ParsingPosition] == ':')
			{
				++m_ParsingPosition;
				m_Params.push_back(m_ParsingBuffer.substr(m_ParsingPosition, m_ParsingBuffer.size() - m_ParsingPosition));
				m_ParsingPosition = m_ParsingBuffer.size();
			}
			else
			{
				size_t SpacePosition = m_ParsingBuffer.find(' ', m_ParsingPosition);

				if(SpacePosition == std::string::npos)
				{
					m_Params.push_back(m_ParsingBuffer.substr(m_ParsingPosition, m_ParsingBuffer.size() - m_ParsingPosition));
					m_ParsingPosition = m_ParsingBuffer.size();
				}
				else
				{
					m_Params.push_back(m_ParsingBuffer.substr(m_ParsingPosition, SpacePosition - m_ParsingPosition));
					m_ParsingPosition = SpacePosition + 1;

				}
			}
		}
	}

	//------------------------------------------//
	//											//
	//				  Process					//
	//											//
	//------------------------------------------//

	void CParser::ProcessParsedMessage()
	{
		ProcessFunctionMap::const_iterator Entry = m_ProcessFunctionMap.find(m_Command);

		if(Entry != CParser::m_ProcessFunctionMap.end())
		{
			(this->*(Entry->second))();
		}
	}
	
	// --- Welcome Messages --- //
	void CParser::ProcessWelcome()
	{
		IRCMessage Message;
		Message.Type = Welcome;

		m_MessageBuffer.push_back(Message);
	}

	// --- Message of the Day --- //
	void CParser::ProcessMOTD()
	{
		m_MOTDBuffer += m_Params[m_Params.size() - 1] + CParser::CommandEnd;
	}

	void CParser::ProcessMOTDEnd()
	{
		IRCMessage Message;
		Message.Type = MOTD;
		Message.Data.MOTD.Message = m_MOTDBuffer;
		m_MOTDBuffer.assign("");
		m_MessageBuffer.push_back(Message);
	}

	// ---User Lists --- //
	void CParser::ProcessNameReply()
	{
		if(m_Params.size() > 1)
		{
			std::string & Channel = m_Params[m_Params.size() - 2];
			std::string & Nicks = m_Params[m_Params.size() - 1];
			StringVector & NickVector = m_UserListBuffer[Channel];

			size_t ParsePosition = 0;
			size_t SpacePosition;

			while(ParsePosition < Nicks.size())
			{
				SpacePosition = Nicks.find(' ', ParsePosition);
				if(SpacePosition != std::string::npos)
				{
					NickVector.push_back(Nicks.substr(ParsePosition, SpacePosition - ParsePosition));
					ParsePosition = SpacePosition + 1;
				}
				else
				{
					NickVector.push_back(Nicks.substr(ParsePosition, Nicks.size() - ParsePosition));
					ParsePosition = Nicks.size();
				}
			}
		}
	}

	void CParser::ProcessNameReplyEnd()
	{
		if(m_Params.size() > 1)
		{
			std::string & Channel = m_Params[m_Params.size() - 2];
			StringVectorMap::iterator MapEntry = m_UserListBuffer.find(Channel);

			if(MapEntry != m_UserListBuffer.end())
			{
				IRCMessage Message;
				Message.Type = UserList;

				Message.Data.UserList.Channel = Channel;
				Message.Data.UserList.Nicks = MapEntry->second;

				m_UserListBuffer.erase(MapEntry);

				m_MessageBuffer.push_back(Message);
			} 
		}
	}

	// --- Ping --- //
	void CParser::ProcessPing()
	{
		if(m_Params.size() > 0)
		{
			m_ResponseBuffer += "PONG ";
			m_ResponseBuffer += m_Params[m_Params.size() - 1] + CParser::CommandEnd;
		}
	}

	// --- Nick Change --- //
	void CParser::ProcessNick()
	{
		IRCMessage Message;

		Message.Type = NickChange; 
		Message.Data.NickChange.Nick = m_PrefixNick;
		Message.Data.NickChange.User = m_PrefixUser;
		Message.Data.NickChange.Host = m_PrefixHost;
		Message.Data.NickChange.NewNick = m_Params[0];

		m_MessageBuffer.push_back(Message);
	}

	// --- Join, Part, Quit --- //
	void  CParser::ProcessJoin()
	{
		if(m_Params.size() > 0)
		{
			IRCMessage Message;

			Message.Type = Join;
			Message.Data.Join.Nick = m_PrefixNick;
			Message.Data.Join.User = m_PrefixUser;
			Message.Data.Join.Host = m_PrefixHost;
			Message.Data.Join.Channel = m_Params[0];

			m_MessageBuffer.push_back(Message);
		}
	}

	void CParser::ProcessPart()
	{
		if(m_Params.size() > 0)
		{
			IRCMessage Message;

			Message.Type = Part;
			Message.Data.Part.Nick = m_PrefixNick;
			Message.Data.Part.User = m_PrefixUser;
			Message.Data.Part.Host = m_PrefixHost;
			Message.Data.Part.Channel = m_Params[0];

			if(m_Params.size() > 1)
			{
				Message.Data.Part.Message = m_Params[m_Params.size() - 1];
			}

			m_MessageBuffer.push_back(Message);
		}
	}

	void CParser::ProcessQuit()
	{
		IRCMessage Message;

		Message.Type = Quit;
		Message.Data.Quit.Nick = m_PrefixNick;
		Message.Data.Quit.User = m_PrefixUser;
		Message.Data.Quit.Host = m_PrefixHost;
		
		if(m_Params.size() > 0)
		{
			Message.Data.Quit.Message = m_Params[m_Params.size() - 1];
		}
		
		m_MessageBuffer.push_back(Message);
		
	}

	// --- Test Messages --- //
	void CParser::ProcessMessage()
	{
		if(m_Params.size() > 1)
		{
			IRCMessage Message;

			Message.Type = Message;
			Message.Data.Message.Nick = m_PrefixNick;
			Message.Data.Message.User = m_PrefixUser;
			Message.Data.Message.Host = m_PrefixHost;
			Message.Data.Message.Reciever = m_Params[0];
			Message.Data.Message.Message = m_Params[m_Params.size() - 1];

			m_MessageBuffer.push_back(Message);
		}
	}

	void CParser::ProcessNotice()
	{	
		if(m_Params.size() > 1)
		{
			IRCMessage Message;

			Message.Type = Notice;
			Message.Data.Message.Nick = m_PrefixNick;
			Message.Data.Message.User = m_PrefixUser;
			Message.Data.Message.Host = m_PrefixHost;
			Message.Data.Message.Reciever = m_Params[0];
			Message.Data.Message.Message = m_Params[m_Params.size() - 1];

			m_MessageBuffer.push_back(Message);
		}

	}


	// --- Modes --- //
	void CParser::ProcessMode()
	{
		std::string & Reciever = m_Params[0];
		bool ChannelMode = ((Reciever[0] == '#') || (Reciever[0] == '&'));
		bool SetMode = (m_Params[1][0] == '+');

		for(std::string::iterator it = (m_Params[1].begin() + 1); it != m_Params[1].end(); ++it)
		{
			IRCMessage Message;

			if(ChannelMode)
			{

				Message.Type = ChannelMode;
				Message.Data.ChannelMode.SenderNick = m_PrefixNick;
				Message.Data.ChannelMode.SenderUser = m_PrefixUser;
				Message.Data.ChannelMode.SenderHost = m_PrefixHost;
				Message.Data.ChannelMode.Channel = Reciever;
				Message.Data.ChannelMode.SetMode = SetMode;

				if(m_Params.size() > 2)
				{
					Message.Data.ChannelMode.Param = m_Params[2];
				}

				switch(*it)
				{
					case 'o':
						Message.Data.ChannelMode.Mode = Operator;
						break;
					case 'p':
						Message.Data.ChannelMode.Mode = Privat;
						break;
					case 's':
						Message.Data.ChannelMode.Mode = Secret;
						break;
					case 'i':
						Message.Data.ChannelMode.Mode = InviteOnly;
						break;
					case 't':
						Message.Data.ChannelMode.Mode = TopicSettableOnlyByOps;
						break;
					case 'n':
						Message.Data.ChannelMode.Mode = NoMessagesFromOutside;
						break;
					case 'm':
						Message.Data.ChannelMode.Mode = Moderated;
						break;
					case 'l':
						Message.Data.ChannelMode.Mode = UserLimit;
						break;
					case 'b':
						Message.Data.ChannelMode.Mode = Ban;
						break;
					case 'v':
						Message.Data.ChannelMode.Mode = VerboseUser;
						break;
					case 'k':
						Message.Data.ChannelMode.Mode = Key;
						break;
				}
			}
			else
			{

				Message.Type = UserMode;
				Message.Data.UserMode.SenderNick = m_PrefixNick;
				Message.Data.UserMode.SenderUser = m_PrefixUser;
				Message.Data.UserMode.SenderHost = m_PrefixHost;
				Message.Data.UserMode.AffectedNick = Reciever;
				Message.Data.UserMode.SetMode = SetMode;

				switch(*it)
				{
					case 'i':
						Message.Data.ChannelMode.Mode = Invisible;
						break;
					case 's':
						Message.Data.ChannelMode.Mode = ServerNotices;
						break;
					case 'w':
						Message.Data.ChannelMode.Mode = Wallops;
						break;
					case 'o':
						Message.Data.ChannelMode.Mode = Operator;
						break;
				}
			}

			m_MessageBuffer.push_back(Message);
		}

		if((m_Params[0][0] == '#') || (m_Params[0][0] == '&'))
		{
			//ChannelMode
			UserMode = false;

			Message.Type = ChannelMode;
			Message.Data.ChannelMode.SenderNick = m_PrefixNick;
			Message.Data.ChannelMode.SenderUser = m_PrefixUser;
			Message.Data.ChannelMode.SenderHost = m_PrefixHost;
			Message.Data.ChannelMode.AffectedNick = m_Params[0];
		}
		else
		{
			//UserMode
			UserMode = true;

			Message.Type = UserMode;
			Message.Data.UserMode.SenderNick = m_PrefixNick;
			Message.Data.UserMode.SenderUser = m_PrefixUser;
			Message.Data.UserMode.SenderHost = m_PrefixHost;
			Message.Data.UserMode.AffectedNick = m_Params[0];
		}

		m_MessageBuffer.push_back(Message);
	}

	// --- Topic --- //
	void CParser::ProcessTopic()
	{
		IRCMessage Message;

		Message.Type = Topic; 
		Message.Data.Topic.Nick = m_PrefixNick;
		Message.Data.Topic.User = m_PrefixUser;
		Message.Data.Topic.Host = m_PrefixHost;
		Message.Data.Topic.Channel = m_Params[0];
		Message.Data.Topic.NewTopic = m_Params[1];

		m_MessageBuffer.push_back(Message);
		
	}

	// --- Kick --- //
	void CParser::ProcessKick()
	{
		IRCMessage Message;

		Message.Type = Kick; 
		Message.Data.Kick.Nick = m_PrefixNick;
		Message.Data.Kick.User = m_PrefixUser;
		Message.Data.Kick.Host = m_PrefixHost;
		Message.Data.Kick.Channel = m_Params[0];
		Message.Data.Kick.Victim = m_Params[1];

		if(m_Params.size() > 2)
		{
			Message.Data.Kick.Reason = m_Params[2];
		}

		m_MessageBuffer.push_back(Message);
	}

	//------------------------------------------//
	//											//
	//				  Register					//
	//											//
	//------------------------------------------//
	
	void CParser::Register(const std::string & Nick, const std::string & RealName, std::string & Package) const
	{
		if(Nick.empty())
		{
			throw IRCNoNickGiven();
		}

		if(Nick.size() > CParser::MaxNickLength)
		{
			throw IRCNickTooLong();
		}

		if(((Nick[0] < 'a') || (Nick[0] > 'z')) && ((Nick[0] < 'A') || (Nick[0] > 'Z')))
		{
			throw IRCInvalidFirstNickCharacter();
		}

		if(Nick.find_first_not_of(CParser::AllowedCharsForNick) != std::string::npos)
		{
			throw IRCInvalidNickCharacters();
		}

		if(RealName.empty())
		{
			throw IRCNoRealnameGiven();
		}

		if(RealName.find_first_of(CParser::NotAllowedCharsForMessages) != std::string::npos)
		{
			throw IRCInvalidRealnameCharacters();
		}

		//Nick Command
		Package += "NICK ";
		Package += Nick + CParser::CommandEnd;

		//User Command
		Package += "USER ";
		Package += Nick + " NULL NULL :" + RealName + CParser::CommandEnd;
	}

	//------------------------------------------//
	//											//
	//				   Join 					//
	//											//
	//------------------------------------------//

	void CParser::JoinChannel(const std::string & ChannelName, const std::string & ChannelPass, std::string & Package) const
	{
		if(ChannelName.empty())
		{
			throw IRCNoChannelNameGiven();
		}

		if(ChannelName.size() > CParser::MaxChannelNameLength)
		{
			throw IRCChannelNameTooLong();
		}

		if((ChannelName[0] != '#') && (ChannelName[0] != '&'))
		{
			throw IRCInvalidFirstChannelNameCharacter();
		}

		if(ChannelName.find_first_of(CParser::NotAllowedCharsForChannel) != std::string::npos)
		{
			throw IRCInvalidChannelNameCharacters();
		}

		if((!ChannelPass.empty()) && (ChannelPass.find_first_of(CParser::NotAllowedCharsForChannel) != std::string::npos))
		{
			throw IRCInvalidChannelPassCharacters();
		}

		Package += "JOIN " + ChannelName;

		if(!ChannelPass.empty())
		{
			Package += " " + ChannelPass;
		}

		Package += CParser::CommandEnd;
	}

	//------------------------------------------//
	//											//
	//			   PrvtMessage 					//
	//											//
	//------------------------------------------//

	void CParser::SendMessage(const std::string & Reciever, const std::string & Message, std::string & Package) const
	{
		if(Reciever.empty())
		{
			throw IRCNoRecieverGiven();
		}

		if(Reciever.size() > CParser::MaxChannelNameLength)
		{
			throw IRCRecieverTooLong();
		}
		if(Reciever.find_first_of(CParser::NotAllowedCharsForChannel) != std::string::npos)
		{
			throw IRCInvalidRecieverCharacters();
		}

		if(Message.find_first_of(CParser::NotAllowedCharsForMessages) != std::string::npos)
		{
			throw IRCInvalidMessageCharacters();
		}

		if(Reciever.size() + Message.size() + 12)
		{
			throw IRCMessageTooLong();
		}

		Package += "PRIVMSG " + Reciever + " :" + Message + CParser::CommandEnd
	}

	//------------------------------------------//
	//											//
	//				   Quit 					//
	//											//
	//------------------------------------------//

	void CParser::GetClosingPackage(const std::string & ExitMessage, std::string & Package) const
	{
		if(ExitMessage.find_first_of(CParser::NotAllowedCharsForMessages) != std::string::npos)
		{
			throw IRCInvalidExitMsgCharacters();
		}

		if((ExitMessage.size() + 8) > CParser::MaxIRCMessageLength)
		{
			throw IRCMessageTooLong();
		}

		Package += "QUIT";

		if(!ExitMessage.empty())
		{
			Package += " :" + ExitMessage;
		}

		Package += CParser::CommandEnd;
	}
}