#pragma once

#include <string>
#include <vector>
#include <map>

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
		typedef void (CParser::*ProcessFunctinPointer)();
		typedef std::map<std::string, ProcessFunctinPointer> ProcessFunctionMap;

		static const size_t MaxNickLength;
		static const std::string AllowedCharsForNick;
		static const size_t MaxChannelNameLength;
		static const std::string NotAllowedCharsForChannel;
		static const std::string NotAllowedCharsForMessages;
		static const std::string CommandEnd;

		static const ProcessFunctionMap m_ProcessFunctionMap;

		std::string m_RecieveBuffer;

		IRCMessageVector m_MessageBuffer;
		std::string m_ResponseBuffer;
		bool m_CloseConnection;

		std::string m_ParsingBuffer;
		size_t m_ParsingPosition;
		
		std::string m_PrefixNick, m_PrefixUser, m_PrefixHost, m_Command;
		std::vector<std::string> m_Params;

		void ParseMessage();
		void ParsePrefix();
		void ParseCommand();
		void ParseParams();

		void ProcessMessage();
		void ProcessWelcome();
		void ProcessPing();
	};
}