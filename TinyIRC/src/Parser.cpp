#include "Parser.h"

namespace tinyirc
{

	const size_t CParser::MaxNickLength = 9;
	const size_t CParser::MaxChannelNameLength = 200;
	const std::string CParser::AllowedCharsForNick = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789{}[]\\|^-_`";
	const std::string CParser::NotAllowedCharsForChannel = "\0\a\r\n ,";
	const std::string CParser::NotAllowedCharsForMessages = "\0\r\n";
	const std::string CParser::CommandEnd = "\r\n";

	const CParser::ProcessFunctionMap CParser::m_ProcessFunctionMap = {
														{"001", &CParser::ProcessWelcome}, 
														{"RPL_WELCOME", &CParser::ProcessWelcome},
														{"PING", &CParser::ProcessPing}
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
			ProcessMessage();
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

	void CParser::ProcessMessage()
	{
		ProcessFunctionMap::const_iterator Entry = m_ProcessFunctionMap.find(m_Command);

		if(Entry != CParser::m_ProcessFunctionMap.end())
		{
			(this->*(Entry->second))();
		}
	}
	
	void CParser::ProcessWelcome()
	{
		IRCMessage Message;
		Message.Type = Welcome;

		m_MessageBuffer.push_back(Message);
	}

	void CParser::ProcessPing()
	{
		if(m_Params.size() > 0)
		{
			m_ResponseBuffer += "PONG ";
			m_ResponseBuffer += m_Params[m_Params.size() - 1] + CParser::CommandEnd;
		}
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
	//				   Close 					//
	//											//
	//------------------------------------------//

	void CParser::JoinChannel(const std::string & ChannelName, const std::string & ChannelPass, std::string & Package)
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

		Package+= CParser::CommandEnd;
	}

	//------------------------------------------//
	//											//
	//				   Close 					//
	//											//
	//------------------------------------------//

	void CParser::GetClosingPackage(const std::string & ExitMessage, std::string & Package) const
	{
		if(ExitMessage.find_first_of(CParser::NotAllowedCharsForMessages) != std::string::npos)
		{
			throw IRCInvalidRealnameCharacters();
		}

		Package += "QUIT";

		if(!ExitMessage.empty())
		{
			Package += " :" + ExitMessage;
		}
	}
}