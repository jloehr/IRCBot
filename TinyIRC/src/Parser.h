#pragma once

#include <string>
#include <vector>

#include "IRCMessage.h"
#include "IRCExceptions.h"

namespace tinyirc
{
	typedef std::vector<IRCMessage> IRCMessageVector;

	class CParser
	{
	public:
		CParser();
		~CParser();

		void Reset();

		bool Parse(const std::string & Recieved, std::string & Response, IRCMessageVector & Messages);

		void Register(const std::string & Nick, const std::string & RealName, std::string & Package) const;
		void JoinChannel(const std::string & ChannelName, const std::string & ChannelPass, std::string & Package);
		void GetClosingPackage(const std::string & ExitMessage, std::string & Package) const;



	private:
		static const size_t MaxNickLength;
		static const std::string AllowedCharsForNick;
		static const size_t MaxChannelNameLength;
		static const std::string NotAllowedCharsForChannel;
		static const std::string NotAllowedCharsForMessages;
		static const std::string CommandEnd;

		std::string m_RecieveBuffer;
		std::string m_ResponseBuffer;

		bool m_Registered;
	};
}