#pragma once

namespace tinyirc
{
	enum IRCMessageType { Welcome, MOTD, NickChange, UserList, Message, Notice, Join, Part, Quit, UserMode, ChannelMode, Topic, Kick};
	enum IRCUserMode { Invisible, ServerNotices, Wallops, Operator};
	enum IRCChannelMode { Operator, Privat, Secret, InviteOnly, TopicSettableOnlyByOps, NoMessagesFromOutside, Moderated, UserLimit, Ban, VerboseUser, Key };
	
	struct IRCMessage
	{
	public:
		IRCMessageType Type;

		union
		{
			struct  
			{
				std::string Message;
			} MOTD;

			struct
			{
				std::string  Nick, User, Host, NewNick;
			} NickChange;

			struct 
			{
				std::string Channel;
				std::std::vector<std::string> Nicks;
			} UserList;

			struct
			{
				std::string Nick, User, Host, Reciever, Message;
			} Message;

			struct
			{
				std::string  Nick, User, Host, Channel;
			} Join;

			struct
			{
				std::string  Nick, User, Host, Channel, Message;
			} Part;

			struct
			{
				std::string  Nick, User, Host, Message;
			} Quit;

			struct
			{
				std::string SenderNick, SenderUser, SenderHost;
				std::string AffectedNick;
				bool SetMode;
				IRCUserMode Mode;
			} UserMode;

			struct
			{
				std::string SenderNick, SenderUser, SenderHost;
				std::string Channel;
				bool SetMode;
				IRCChannelMode Mode;
				std::string Param
			} ChannelMode;

			struct
			{
				std::string  Nick, User, Host, Channel, NewTopic;
			} Topic;

			struct
			{
				std::string Nick, User, Host, Channel, Victim, Reason;
			} Kick;
		} Data;
	};
}