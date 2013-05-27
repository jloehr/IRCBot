#include "Parser.h"

namespace tinyirc
{

	const size_t CParser::MaxNickLength = 9;
	const size_t CParser::MaxChannelNameLength = 200;
	const std::string CParser::AllowedCharsForNick = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789{}[]\\|^-_`";
	const std::string CParser::NotAllowedCharsForChannel = "\0\a\r\n ,";
	const std::string CParser::NotAllowedCharsForMessages = "\0\r\n";
	const std::string CParser::CommandEnd = "\r\n";

	//------------------------------------------//
	//											//
	//		  Konstructor & Destructor			//
	//											//
	//------------------------------------------//

	CParser::CParser()
		:m_Registered(false)
	{

	}

	CParser::~CParser()
	{

	}

	void CParser::Reset()
	{
		m_Registered = false;
		m_RecieveBuffer.assign("");
		m_ResponseBuffer.assign("");
	}

	//------------------------------------------//
	//											//
	//				   Parse 					//
	//											//
	//------------------------------------------//

	bool CParser::Parse(const std::string & Recieved, std::string & Response, IRCMessageVector & Messages)
	{
		m_RecieveBuffer += Recieved;

		size_t EndPos;

		while((EndPos = m_RecieveBuffer.find(CParser::CommandEnd)) != std::string::npos)
		{
			std::string Message(m_RecieveBuffer, 0, EndPos + CParser::CommandEnd.size());
			m_RecieveBuffer.erase(0, EndPos + CParser::CommandEnd.size());

			size_t SpacePositon = Message.find(' ');

			std::string Command(Message, 0, SpacePositon);

			std::string Ping("PING");

			if(Command.compare(Ping) == 0)
			{
				size_t ParamPos = Message.rfind(':');
				Response += "PONG " + Message.substr(ParamPos + 1);
			}
		}

		if(!m_ResponseBuffer.empty())
		{
			Response += m_ResponseBuffer;
			m_ResponseBuffer.assign("");
		}

		return false;
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

		std::string & Buffer = (m_Registered) ? Package : m_ResponseBuffer;

		Buffer += "JOIN " + ChannelName;

		if(!ChannelPass.empty())
		{
			Buffer += " " + ChannelPass;
		}

		Buffer+= CParser::CommandEnd;
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