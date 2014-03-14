/*
 *
 * Copyright (C) 2013 Julian Löhr
 * All rights reserved.
 *
 * This file is part of TinyIRC, an IRC Client Implementation.
 * TinyIRC is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
 * To read the full license, please check the LICENSE file.
 *
*/
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

														{"353", &CParser::ProcessNameReply}, 
														{"RPL_NAMREPLY", &CParser::ProcessNameReply},
														{"366", &CParser::ProcessNameReplyEnd}, 
														{"RPL_ENDOFNAMES", &CParser::ProcessNameReplyEnd},
														{"331", &CParser::ProcessTopic}, 
														{"RPL_NOTOPIC", &CParser::ProcessTopic}, 
														{"332", &CParser::ProcessTopic}, 
														{"RPL_TOPIC", &CParser::ProcessTopic}, 

														{"JOIN", &CParser::ProcessJoin},
														{"PART", &CParser::ProcessPart},
														{"QUIT", &CParser::ProcessQuit},

														{"PRIVMSG", &CParser::ProcessMessage}, 
														{"NOTICE", &CParser::ProcessNotice},

														{"MODE", &CParser::ProcessMode}, 
														{"TOPIC", &CParser::ProcessTopicChanged}, 
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
		Message.Type = IRCMessageType::Welcome;
		Message.Data.Welcome.Message = new std::string(m_Params[m_Params.size() - 1]);

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
		Message.Type = IRCMessageType::MOTD;
		Message.Data.MOTD.Message = new std::string(m_MOTDBuffer);
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
				Message.Type = IRCMessageType::UserList;

				Message.Data.UserList.Channel = new std::string(Channel);
				Message.Data.UserList.Nicks = new StringVector(MapEntry->second);

				m_UserListBuffer.erase(MapEntry);

				m_MessageBuffer.push_back(Message);
			} 
		}
	}

	// --- Topic --- //
	void CParser::ProcessTopic()
	{
		IRCMessage Message;

		Message.Type = IRCMessageType::Topic;
		Message.Data.Topic.Channel = new std::string(m_Params[0]);
		Message.Data.Topic.Topic = new std::string(m_Params[1]);

		m_MessageBuffer.push_back(Message);
		
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

		Message.Type = IRCMessageType::NickChange; 
		Message.Data.NickChange.Nick = new std::string(m_PrefixNick);
		Message.Data.NickChange.User = new std::string(m_PrefixUser);
		Message.Data.NickChange.Host = new std::string(m_PrefixHost);
		Message.Data.NickChange.NewNick = new std::string(m_Params[0]);

		m_MessageBuffer.push_back(Message);
	}

	// --- Join, Part, Quit --- //
	void  CParser::ProcessJoin()
	{
		if(m_Params.size() > 0)
		{
			IRCMessage Message;

			Message.Type = IRCMessageType::Join;
			Message.Data.Join.Nick = new std::string(m_PrefixNick);
			Message.Data.Join.User = new std::string(m_PrefixUser);
			Message.Data.Join.Host = new std::string(m_PrefixHost);
			Message.Data.Join.Channel = new std::string(m_Params[0]);

			m_MessageBuffer.push_back(Message);
		}
	}

	void CParser::ProcessPart()
	{
		if(m_Params.size() > 0)
		{
			IRCMessage Message;

			Message.Type = IRCMessageType::Part;
			Message.Data.Part.Nick = new std::string(m_PrefixNick);
			Message.Data.Part.User = new std::string(m_PrefixUser);
			Message.Data.Part.Host = new std::string(m_PrefixHost);
			Message.Data.Part.Channel = new std::string(m_Params[0]);
			Message.Data.Part.Message = (m_Params.size() > 1) ? new std::string(m_Params[m_Params.size() - 1]) : new std::string("");	
			m_MessageBuffer.push_back(Message);
		}
	}

	void CParser::ProcessQuit()
	{
		IRCMessage Message;

		Message.Type = IRCMessageType::Quit;
		Message.Data.Quit.Nick = new std::string(m_PrefixNick);
		Message.Data.Quit.User = new std::string(m_PrefixUser);
		Message.Data.Quit.Host = new std::string(m_PrefixHost);
		Message.Data.Quit.Message = (m_Params.size() > 0) ? new std::string(m_Params[m_Params.size() - 1]) : new std::string("");
		
		m_MessageBuffer.push_back(Message);
		
	}

	// --- Test Messages --- //
	void CParser::ProcessMessage()
	{
		if(m_Params.size() > 1)
		{
			IRCMessage Message;

			Message.Type = IRCMessageType::Message;
			Message.Data.Message.Nick = new std::string(m_PrefixNick);
			Message.Data.Message.User = new std::string(m_PrefixUser);
			Message.Data.Message.Host = new std::string(m_PrefixHost);
			Message.Data.Message.Reciever = new std::string(m_Params[0]);
			Message.Data.Message.Message = new std::string(m_Params[m_Params.size() - 1]);

			m_MessageBuffer.push_back(Message);
		}
	}

	void CParser::ProcessNotice()
	{	
		if(m_Params.size() > 1)
		{
			IRCMessage Message;

			Message.Type = IRCMessageType::Notice;
			Message.Data.Message.Nick = new std::string(m_PrefixNick);
			Message.Data.Message.User = new std::string(m_PrefixUser);
			Message.Data.Message.Host = new std::string(m_PrefixHost);
			Message.Data.Message.Reciever = new std::string(m_Params[0]);
			Message.Data.Message.Message = new std::string(m_Params[m_Params.size() - 1]);

			m_MessageBuffer.push_back(Message);
		}

	}


	// --- Modes --- //
	void CParser::ProcessMode()
	{
		if((m_Params[1][0] != '+') && (m_Params[1][0] != '-'))
		{
			return;
		}

		std::string & Reciever = m_Params[0];
		bool ChannelMode = ((Reciever[0] == '#') || (Reciever[0] == '&'));
		bool SetMode = (m_Params[1][0] == '+');

		size_t ParamIndex = 2;

		for(std::string::iterator it = (m_Params[1].begin() + 1); it != m_Params[1].end(); ++it)
		{
			if(((*it) == '+') || ((*it) == '-'))
			{
				SetMode = ((*it) == '+');
				continue;
			}

			IRCMessage Message;

			if(ChannelMode)
			{

				Message.Type = IRCMessageType::ChannelMode;
				Message.Data.ChannelMode.SenderNick = new std::string(m_PrefixNick);
				Message.Data.ChannelMode.SenderUser = new std::string(m_PrefixUser);
				Message.Data.ChannelMode.SenderHost = new std::string(m_PrefixHost);
				Message.Data.ChannelMode.Channel = new std::string(Reciever);
				Message.Data.ChannelMode.SetMode = SetMode;
				Message.Data.ChannelMode.ModeAsChar = (*it);
				//Message.Data.ChannelMode.Param = (m_Params.size() >= ParamIndex) ? new std::string(m_Params[2]) : new std::string("");

				switch(*it)
				{
					case 'o':
						Message.Data.ChannelMode.Mode = IRCChannelMode::Operator;
						Message.Data.ChannelMode.Param = (ParamIndex < m_Params.size()) ? new std::string(m_Params[ParamIndex++]) : new std::string("");
						break;
					case 'p':
						Message.Data.ChannelMode.Mode = IRCChannelMode::Privat;
						Message.Data.ChannelMode.Param = new std::string("");
						break;
					case 's':
						Message.Data.ChannelMode.Mode = IRCChannelMode::Secret;
						Message.Data.ChannelMode.Param = new std::string("");
						break;
					case 'i':
						Message.Data.ChannelMode.Mode = IRCChannelMode::InviteOnly;
						Message.Data.ChannelMode.Param = new std::string("");
						break;
					case 't':
						Message.Data.ChannelMode.Mode = IRCChannelMode::TopicSettableOnlyByOps;
						Message.Data.ChannelMode.Param = new std::string("");
						break;
					case 'n':
						Message.Data.ChannelMode.Mode = IRCChannelMode::NoMessagesFromOutside;
						Message.Data.ChannelMode.Param = new std::string("");
						break;
					case 'm':
						Message.Data.ChannelMode.Mode = IRCChannelMode::Moderated;
						Message.Data.ChannelMode.Param = new std::string("");
						break;
					case 'l':
						Message.Data.ChannelMode.Mode = IRCChannelMode::UserLimit;
						Message.Data.ChannelMode.Param = (ParamIndex < m_Params.size()) ? new std::string(m_Params[ParamIndex++]) : new std::string("");
						break;
					case 'b':
						Message.Data.ChannelMode.Mode = IRCChannelMode::Ban;
						Message.Data.ChannelMode.Param = (ParamIndex < m_Params.size()) ? new std::string(m_Params[ParamIndex++]) : new std::string("");
						break;
					case 'v':
						Message.Data.ChannelMode.Mode = IRCChannelMode::Voice;
						Message.Data.ChannelMode.Param = (ParamIndex < m_Params.size()) ? new std::string(m_Params[ParamIndex++]) : new std::string("");
						break;
					case 'k':
						Message.Data.ChannelMode.Mode = IRCChannelMode::Key;
						Message.Data.ChannelMode.Param = (ParamIndex < m_Params.size()) ? new std::string(m_Params[ParamIndex++]) : new std::string("");
						break;
					default:
						Message.Data.ChannelMode.Mode = IRCChannelMode::Undefined;
						Message.Data.ChannelMode.Param = new std::string("");

				}
			}
			else
			{

				Message.Type = IRCMessageType::UserMode;
				Message.Data.UserMode.SenderNick = new std::string(m_PrefixNick);
				Message.Data.UserMode.SenderUser = new std::string(m_PrefixUser);
				Message.Data.UserMode.SenderHost = new std::string(m_PrefixHost);
				Message.Data.UserMode.AffectedNick = new std::string(Reciever);
				Message.Data.UserMode.SetMode = SetMode;
				Message.Data.UserMode.ModeAsChar = (*it);

				switch(*it)
				{
					case 'i':
						Message.Data.UserMode.Mode = IRCUserMode::Invisible;
						break;
					case 's':
						Message.Data.UserMode.Mode = IRCUserMode::ServerNotices;
						break;
					case 'w':
						Message.Data.UserMode.Mode = IRCUserMode::Wallops;
						break;
					case 'o':
						Message.Data.UserMode.Mode = IRCUserMode::Operator;
						break;
					default:
						Message.Data.UserMode.Mode = IRCUserMode::Undefined;
				}
			}

			m_MessageBuffer.push_back(Message);
		}
	}

	// --- Topic Changed --- //
	void CParser::ProcessTopicChanged()
	{
		if(m_Params.size() >= 1)
		{
			IRCMessage Message;

			Message.Type = IRCMessageType::TopicChanged; 
			Message.Data.TopicChanged.Nick = new std::string(m_PrefixNick);
			Message.Data.TopicChanged.User = new std::string(m_PrefixUser);
			Message.Data.TopicChanged.Host = new std::string(m_PrefixHost);
			Message.Data.TopicChanged.Channel = new std::string(m_Params[0]);
			Message.Data.TopicChanged.NewTopic = (m_Params.size() >= 2) ? new std::string(m_Params[1]) : new std::string("");

			m_MessageBuffer.push_back(Message);
		}	
	}

	// --- Kick --- //
	void CParser::ProcessKick()
	{
		if(m_Params.size() >= 2)
		{
			IRCMessage Message;

			Message.Type = IRCMessageType::Kick; 
			Message.Data.Kick.Nick = new std::string(m_PrefixNick);
			Message.Data.Kick.User = new std::string(m_PrefixUser);
			Message.Data.Kick.Host = new std::string(m_PrefixHost);
			Message.Data.Kick.Channel = new std::string(m_Params[0]);
			Message.Data.Kick.Victim = new std::string(m_Params[1]);
			Message.Data.Kick.Reason = (m_Params.size() >= 3) ? new std::string(m_Params[2]) : new std::string("");

			m_MessageBuffer.push_back(Message);
		}
	}

	//------------------------------------------//
	//											//
	//				  Register					//
	//											//
	//------------------------------------------//
	
	void CParser::Register(const std::string & Nick, const std::string & RealName, std::string & Package) const
	{
		Register(std::string(), Nick, RealName, Package);
	}

	void CParser::Register( const std::string & ConnectionPassword, const std::string & Nick, const std::string & RealName, std::string & Package) const
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

		if(!ConnectionPassword.empty())
		{
			if(ConnectionPassword.find_first_of(CParser::NotAllowedCharsForMessages) != std::string::npos)
			{
				throw IRCInvalidConnectionPassCharacter();
			}

			//Pass Command
			Package += "PASS ";
			Package += ConnectionPassword + CParser::CommandEnd;
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

		if((ChannelName.size() + ChannelPass.size() + 8) > CParser::MaxIRCMessageLength)
		{
			throw IRCMessageTooLong();
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

		if((Reciever.size() + Message.size() + 12) > CParser::MaxIRCMessageLength)
		{
			throw IRCMessageTooLong();
		}

		Package += "PRIVMSG " + Reciever + " :" + Message + CParser::CommandEnd;
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