#pragma once

#include <exception>

namespace tinyirc
{
	class IRCException : public std::exception
	{
	public:
		IRCException(const char * WhatMessage) noexcept
			:m_WhatMessage(WhatMessage)	{ }

		IRCException (const IRCException & exception) noexcept
			:m_WhatMessage(exception.m_WhatMessage) {}

		virtual ~IRCException() noexcept {}

		virtual const char * what() const noexcept
		{
			return m_WhatMessage;
		}

	private:

		const char * m_WhatMessage;
	};

	#define IRCDerivedException(ExceptionName, Message)					\
																		\
	class ExceptionName : public IRCException							\
	{																	\
	public:																\
		ExceptionName()	noexcept										\
			:IRCException(Message)										\
		{ }																\
																		\
		ExceptionName(const ExceptionName & exception)	noexcept		\
			:IRCException(exception)									\
		{ }																\
	};																	\

	//Nickname
	IRCDerivedException(IRCNickTooLong, "The given Nickname is too long. It can be up to 9 Characters long.")
	IRCDerivedException(IRCNoNickGiven, "The given Nickname is emtpy!")
	IRCDerivedException(IRCInvalidFirstNickCharacter, "In Nickname has to start with a letter!")
	IRCDerivedException(IRCInvalidNickCharacters, "In the given Nickname are invalid Characters found!")

	//Realname
	IRCDerivedException(IRCNoRealnameGiven, "The given Realname is empty!")
	IRCDerivedException(IRCInvalidRealnameCharacters, "In the given Realname are invalid Characters found!")

	//Channelname
	IRCDerivedException(IRCNoChannelNameGiven, "The given Channel Name is empty!")
	IRCDerivedException(IRCChannelNameTooLong, "The given Channel Name is too long. It can be up to 200 Characters long.")
	IRCDerivedException(IRCInvalidFirstChannelNameCharacter, "The Channel Name has to start with a \"#\" or \"&\"")
	IRCDerivedException(IRCInvalidChannelNameCharacters, "In the given Channel Name are invalid Characters found!")
	
	//ChannelPass
	IRCDerivedException(IRCInvalidChannelPassCharacters, "In the given Channel Password are invalid Characters found!")
	
	//Reciever
	IRCDerivedException(IRCNoRecieverGiven, "The given Reciever is empty!")
	IRCDerivedException(IRCRecieverTooLong, "The given Reciever is too long. It can be up to 200 Characters long.")
	IRCDerivedException(IRCInvalidRecieverCharacters, "In the given Reciever are invalid Characters found!")
	
	//Message
	IRCDerivedException(IRCNoMessageGiven, "The given Message is empty!")
	IRCDerivedException(IRCInvalidMessageCharacters, "In the given Exit Message are invalid Characters found!")

	//ExitMsg
	IRCDerivedException(IRCInvalidExitMsgCharacters, "In the given Exit Message are invalid Characters found!")

	//Message
	IRCDerivedException(IRCMessageTooLong, "The IRC Message would exceed the maximun length of 512 Characters.")

}