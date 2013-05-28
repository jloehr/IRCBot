#pragma once

#include <strings.h>
#include <string>
#include <vector>

namespace tinyirc
{
	namespace IRCMessageType
	{
		enum Type { Undefined, Welcome, MOTD, NickChange, UserList, Message, Notice, Join, Part, Quit, UserMode, ChannelMode, Topic, Kick};
	}

	namespace IRCUserMode
	{
		enum Type { Invisible, ServerNotices, Wallops, Operator};
	}

	namespace IRCChannelMode
	{
		enum Type { Operator, Privat, Secret, InviteOnly, TopicSettableOnlyByOps, NoMessagesFromOutside, Moderated, UserLimit, Ban, VerboseUser, Key };
	}

	struct IRCMessage
	{
	public:
		IRCMessageType::Type Type;

		union
		{
			struct  
			{
				std::string * Message;
			} MOTD;

			struct
			{
				std::string  * Nick, * User, * Host, * NewNick;
			} NickChange;

			struct 
			{
				std::string * Channel;
				std::vector<std::string> * Nicks;
			} UserList;

			struct
			{
				std::string * Nick, * User, * Host, * Reciever, * Message;
			} Message;

			struct
			{
				std::string * Nick, * User, * Host, * Channel;
			} Join;

			struct
			{
				std::string * Nick, * User, * Host, * Channel, * Message;
			} Part;

			struct
			{
				std::string * Nick, * User, * Host, * Message;
			} Quit;

			struct
			{
				std::string * SenderNick, * SenderUser, * SenderHost;
				std::string * AffectedNick;
				bool SetMode;
				IRCUserMode::Type Mode;
			} UserMode;

			struct
			{
				std::string * SenderNick, * SenderUser, * SenderHost;
				std::string * Channel;
				bool SetMode;
				IRCChannelMode::Type Mode;
				std::string * Param;
			} ChannelMode;

			struct
			{
				std::string * Nick, * User, * Host, * Channel, * NewTopic;
			} Topic;

			struct
			{
				std::string * Nick, * User, * Host, * Channel, * Victim, * Reason;
			} Kick;

		} Data;


		IRCMessage()
			:Type(IRCMessageType::Undefined)
		{
			bzero(&Data, sizeof(Data));
		}

		IRCMessage(const IRCMessage & Source)
			:Type(Source.Type)
		{
			bzero(&Data, sizeof(Data));

			switch(Type)
			{
				case IRCMessageType::MOTD:
					Data.MOTD.Message = new std::string(*Source.Data.MOTD.Message);
					break;

				case IRCMessageType::NickChange:
					Data.NickChange.Nick = new std::string(*Source.Data.NickChange.Nick);
					Data.NickChange.User = new std::string(*Source.Data.NickChange.User);
					Data.NickChange.Host = new std::string(*Source.Data.NickChange.Host);
					Data.NickChange.NewNick = new std::string(*Source.Data.NickChange.NewNick);
					break;

				case IRCMessageType::UserList:
					Data.UserList.Channel = new std::string(*Source.Data.UserList.Channel);
					Data.UserList.Nicks = new std::vector<std::string>(*Source.Data.UserList.Nicks);
					break;

				case IRCMessageType::Message:
				case IRCMessageType::Notice:
					Data.Message.Nick = new std::string(*Source.Data.Message.Nick);
					Data.Message.User = new std::string(*Source.Data.Message.User);
					Data.Message.Host = new std::string(*Source.Data.Message.Host);
					Data.Message.Reciever = new std::string(*Source.Data.Message.Reciever);
					Data.Message.Message = new std::string(*Source.Data.Message.Message);
					break;

				case IRCMessageType::Join:
					Data.Join.Nick = new std::string(*Source.Data.Join.Nick);
					Data.Join.User = new std::string(*Source.Data.Join.User);
					Data.Join.Host = new std::string(*Source.Data.Join.Host);
					Data.Join.Channel = new std::string(*Source.Data.Join.Channel);
					break;

				case IRCMessageType::Part:
					Data.Part.Nick = new std::string(*Source.Data.Part.Nick);
					Data.Part.User = new std::string(*Source.Data.Part.User);
					Data.Part.Host = new std::string(*Source.Data.Part.Host);
					Data.Part.Channel = new std::string(*Source.Data.Part.Channel);
					Data.Part.Message = new std::string(*Source.Data.Part.Message);
					break;

				case IRCMessageType::Quit:
					Data.Quit.Nick = new std::string(*Source.Data.Quit.Nick);
					Data.Quit.User = new std::string(*Source.Data.Quit.User);
					Data.Quit.Host = new std::string(*Source.Data.Quit.Host);
					Data.Quit.Message = new std::string(*Source.Data.Quit.Message);
					break;

				case IRCMessageType::UserMode:
					Data.UserMode.SenderNick = new std::string(*Source.Data.UserMode.SenderNick);
					Data.UserMode.SenderUser = new std::string(*Source.Data.UserMode.SenderUser);
					Data.UserMode.SenderHost = new std::string(*Source.Data.UserMode.SenderHost);
					Data.UserMode.AffectedNick = new std::string(*Source.Data.UserMode.AffectedNick);
					break;

				case IRCMessageType::ChannelMode:
					Data.ChannelMode.SenderNick = new std::string(*Source.Data.ChannelMode.SenderNick);
					Data.ChannelMode.SenderUser = new std::string(*Source.Data.ChannelMode.SenderUser);
					Data.ChannelMode.SenderHost = new std::string(*Source.Data.ChannelMode.SenderHost);
					Data.ChannelMode.Channel = new std::string(*Source.Data.ChannelMode.Channel);
					Data.ChannelMode.Param = new std::string(*Source.Data.ChannelMode.Param);
					break;

				case IRCMessageType::Topic:
					Data.Topic.Nick = new std::string(*Source.Data.Topic.Nick);
					Data.Topic.User = new std::string(*Source.Data.Topic.User);
					Data.Topic.Host = new std::string(*Source.Data.Topic.Host);
					Data.Topic.Channel = new std::string(*Source.Data.Topic.Channel);
					Data.Topic.NewTopic = new std::string(*Source.Data.Topic.NewTopic);
					break;

				case IRCMessageType::Kick:
					Data.Kick.Nick = new std::string(*Source.Data.Kick.Nick);
					Data.Kick.User = new std::string(*Source.Data.Kick.User);
					Data.Kick.Host = new std::string(*Source.Data.Kick.Host);
					Data.Kick.Channel = new std::string(*Source.Data.Kick.Channel);
					Data.Kick.Victim = new std::string(*Source.Data.Kick.Victim);
					Data.Kick.Reason = new std::string(*Source.Data.Kick.Reason);
					break;

				default:
					break;
			}
		}

		~IRCMessage()
		{
			switch(Type)
			{
				case IRCMessageType::MOTD:
					Delete(Data.MOTD.Message);
					break;

				case IRCMessageType::NickChange:
					Delete(Data.NickChange.Nick);
					Delete(Data.NickChange.User);
					Delete(Data.NickChange.Host);
					Delete(Data.NickChange.NewNick);
					break;

				case IRCMessageType::UserList:
					Delete(Data.UserList.Channel);
					Delete(Data.UserList.Nicks);
					break;

				case IRCMessageType::Message:
				case IRCMessageType::Notice:
					Delete(Data.Message.Nick);
					Delete(Data.Message.User);
					Delete(Data.Message.Host);
					Delete(Data.Message.Reciever);
					Delete(Data.Message.Message);
					break;

				case IRCMessageType::Join:
					Delete(Data.Join.Nick);
					Delete(Data.Join.User);
					Delete(Data.Join.Host);
					Delete(Data.Join.Channel);
					break;

				case IRCMessageType::Part:
					Delete(Data.Part.Nick);
					Delete(Data.Part.User);
					Delete(Data.Part.Host);
					Delete(Data.Part.Channel);
					Delete(Data.Part.Message);
					break;

				case IRCMessageType::Quit:
					Delete(Data.Quit.Nick);
					Delete(Data.Quit.User);
					Delete(Data.Quit.Host);
					Delete(Data.Quit.Message);
					break;

				case IRCMessageType::UserMode:
					Delete(Data.UserMode.SenderNick);
					Delete(Data.UserMode.SenderUser);
					Delete(Data.UserMode.SenderHost);
					Delete(Data.UserMode.AffectedNick);
					break;

				case IRCMessageType::ChannelMode:
					Delete(Data.ChannelMode.SenderNick);
					Delete(Data.ChannelMode.SenderUser);
					Delete(Data.ChannelMode.SenderHost);
					Delete(Data.ChannelMode.Channel);
					Delete(Data.ChannelMode.Param);
					break;

				case IRCMessageType::Topic:
					Delete(Data.Topic.Nick);
					Delete(Data.Topic.User);
					Delete(Data.Topic.Host);
					Delete(Data.Topic.Channel);
					Delete(Data.Topic.NewTopic);
					break;

				case IRCMessageType::Kick:
					Delete(Data.Kick.Nick);
					Delete(Data.Kick.User);
					Delete(Data.Kick.Host);
					Delete(Data.Kick.Channel);
					Delete(Data.Kick.Victim);
					Delete(Data.Kick.Reason);
					break;

				default:
					break;
			}
		}

	private:
		template<class Type>
		void Delete(Type * & Ptr)
		{
			if(Ptr != NULL)
			{
				delete Ptr;
				Ptr = NULL;
			}
		}
	};
}