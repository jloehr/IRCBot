/*
 *
 * Copyright (C) 2013 Julian Löhr
 * All rights reserved.
 *
 * This file is part of TinyIRC, an IRC Client Implementation.
 * TinyIRC is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * TinyIRC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with TinyIRC.  If not, see <http://www.gnu.org/licenses/>.
 *
*/
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
		void Register( const std::string & ConnectionPassword, const std::string & Nick, const std::string & RealName, std::string & Package) const;
		void JoinChannel(const std::string & ChannelName, const std::string & ChannelPass, std::string & Package) const;
		void SendMessage(const std::string & Reciever, const std::string & Message, std::string & Package) const;
		void GetClosingPackage(const std::string & ExitMessage, std::string & Package) const;

	private:
		typedef void (CParser::*ProcessFunctinPointer)();
		typedef std::map<std::string, ProcessFunctinPointer> ProcessFunctionMap;

		typedef std::vector<std::string> StringVector;
		typedef std::map<std::string, StringVector> StringVectorMap;

		static const size_t MaxIRCMessageLength;
		static const size_t MaxNickLength;
		static const std::string AllowedCharsForNick;
		static const size_t MaxChannelNameLength;
		static const std::string NotAllowedCharsForChannel;
		static const std::string NotAllowedCharsForMessages;
		static const std::string CommandEnd;

		static const ProcessFunctionMap m_ProcessFunctionMap;

		std::string m_RecieveBuffer;

		std::string m_MOTDBuffer;
		StringVectorMap m_UserListBuffer;

		IRCMessageVector m_MessageBuffer;
		std::string m_ResponseBuffer;
		bool m_CloseConnection;

		std::string m_ParsingBuffer;
		size_t m_ParsingPosition;
		
		std::string m_PrefixNick, m_PrefixUser, m_PrefixHost, m_Command;
		StringVector m_Params;

		void ParseMessage();
		void ParsePrefix();
		void ParseCommand();
		void ParseParams();

		void ProcessParsedMessage();
		void ProcessWelcome();
		void ProcessMOTD();
		void ProcessMOTDEnd();
		void ProcessNameReply();
		void ProcessNameReplyEnd();
		void ProcessTopic();

		void ProcessPing();
		void ProcessNick();
		void ProcessJoin();
		void ProcessPart();
		void ProcessQuit();
		void ProcessMessage();
		void ProcessNotice();
		void ProcessMode();
		void ProcessTopicChanged();
		void ProcessKick();
	};
}