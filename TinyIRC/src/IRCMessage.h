#pragma once

namespace tinyirc
{
	enum IRCMessageType { Message };

	struct IRCMessage
	{
	public:
		std::string Sender, Reciever, Data; 
		IRCMessageType Type;
	};
}