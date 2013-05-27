#pragma once

namespace tinyirc
{
	enum IRCMessageType { Welcome, MOTD, Message, Notice, Join, Part, Mode, Quit, Topic, Away, UserList};

	struct IRCMessage
	{
	public:
		IRCMessageType Type;
	};
}