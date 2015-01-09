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

#include <strings.h>
#include <string>
#include <vector>

namespace tinyirc
{
	namespace IRCMessageType
	{
		enum Type { Undefined, Welcome, MOTD, NickChange, UserList, Topic, Message, Notice, Join, Part, Quit, UserMode, ChannelMode, TopicChanged, Kick};
	}

	namespace IRCUserMode
	{
		enum Type { Undefined, Invisible, ServerNotices, Wallops, Operator};
	}

	namespace IRCChannelMode
	{
		enum Type { Undefined, Operator, Privat, Secret, InviteOnly, TopicSettableOnlyByOps, NoMessagesFromOutside, Moderated, UserLimit, Ban, Voice, Key };
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
			} Welcome;

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
				std::string * Channel, * Topic;
			} Topic;

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
				char ModeAsChar;
			} UserMode;

			struct
			{
				std::string * SenderNick, * SenderUser, * SenderHost;
				std::string * Channel;
				bool SetMode;
				IRCChannelMode::Type Mode;
				char ModeAsChar;
				std::string * Param;
			} ChannelMode;

			struct
			{
				std::string * Nick, * User, * Host, * Channel, * NewTopic;
			} TopicChanged;

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
				case IRCMessageType::Welcome:
					Data.Welcome.Message = Copy(Source.Data.Welcome.Message);
					break;

				case IRCMessageType::MOTD:
					Data.MOTD.Message = Copy(Source.Data.MOTD.Message);
					break;

				case IRCMessageType::NickChange:
					Data.NickChange.Nick = Copy(Source.Data.NickChange.Nick);
					Data.NickChange.User = Copy(Source.Data.NickChange.User);
					Data.NickChange.Host = Copy(Source.Data.NickChange.Host);
					Data.NickChange.NewNick = Copy(Source.Data.NickChange.NewNick);
					break;

				case IRCMessageType::UserList:
					Data.UserList.Channel = Copy(Source.Data.UserList.Channel);
					Data.UserList.Nicks = Copy(Source.Data.UserList.Nicks);
					break;

				case IRCMessageType::Topic:
					Data.Topic.Channel = Copy(Source.Data.Topic.Channel);
					Data.Topic.Topic = Copy(Source.Data.Topic.Topic);
					break;

				case IRCMessageType::Message:
				case IRCMessageType::Notice:
					Data.Message.Nick = Copy(Source.Data.Message.Nick);
					Data.Message.User = Copy(Source.Data.Message.User);
					Data.Message.Host = Copy(Source.Data.Message.Host);
					Data.Message.Reciever = Copy(Source.Data.Message.Reciever);
					Data.Message.Message = Copy(Source.Data.Message.Message);
					break;

				case IRCMessageType::Join:
					Data.Join.Nick = Copy(Source.Data.Join.Nick);
					Data.Join.User = Copy(Source.Data.Join.User);
					Data.Join.Host = Copy(Source.Data.Join.Host);
					Data.Join.Channel = Copy(Source.Data.Join.Channel);
					break;

				case IRCMessageType::Part:
					Data.Part.Nick = Copy(Source.Data.Part.Nick);
					Data.Part.User = Copy(Source.Data.Part.User);
					Data.Part.Host = Copy(Source.Data.Part.Host);
					Data.Part.Channel = Copy(Source.Data.Part.Channel);
					Data.Part.Message = Copy(Source.Data.Part.Message);
					break;

				case IRCMessageType::Quit:
					Data.Quit.Nick = Copy(Source.Data.Quit.Nick);
					Data.Quit.User = Copy(Source.Data.Quit.User);
					Data.Quit.Host = Copy(Source.Data.Quit.Host);
					Data.Quit.Message = Copy(Source.Data.Quit.Message);	
					break;

				case IRCMessageType::UserMode:
					Data.UserMode.SenderNick =		Copy(Source.Data.UserMode.SenderNick);
					Data.UserMode.SenderUser = 		Copy(Source.Data.UserMode.SenderUser);
					Data.UserMode.SenderHost = 		Copy(Source.Data.UserMode.SenderHost);
					Data.UserMode.AffectedNick = 	Copy(Source.Data.UserMode.AffectedNick);
					Data.UserMode.SetMode = 		Source.Data.UserMode.SetMode;
					Data.UserMode.ModeAsChar = 		Source.Data.UserMode.ModeAsChar;
					Data.UserMode.Mode = 			Source.Data.UserMode.Mode;
					break;

				case IRCMessageType::ChannelMode:
					Data.ChannelMode.SenderNick = 	Copy(Source.Data.ChannelMode.SenderNick);
					Data.ChannelMode.SenderUser = 	Copy(Source.Data.ChannelMode.SenderUser);
					Data.ChannelMode.SenderHost = 	Copy(Source.Data.ChannelMode.SenderHost);
					Data.ChannelMode.Channel = 		Copy(Source.Data.ChannelMode.Channel);
					Data.ChannelMode.Param = 		Copy(Source.Data.ChannelMode.Param);
					Data.ChannelMode.SetMode = 		Source.Data.ChannelMode.SetMode;
					Data.ChannelMode.ModeAsChar = 	Source.Data.ChannelMode.ModeAsChar;
					Data.ChannelMode.Mode = 		Source.Data.ChannelMode.Mode;
					break;

				case IRCMessageType::TopicChanged:
					Data.TopicChanged.Nick = Copy(Source.Data.TopicChanged.Nick);
					Data.TopicChanged.User = Copy(Source.Data.TopicChanged.User);
					Data.TopicChanged.Host = Copy(Source.Data.TopicChanged.Host);
					Data.TopicChanged.Channel = Copy(Source.Data.TopicChanged.Channel);
					Data.TopicChanged.NewTopic = Copy(Source.Data.TopicChanged.NewTopic);
					break;

				case IRCMessageType::Kick:
					Data.Kick.Nick = Copy(Source.Data.Kick.Nick);
					Data.Kick.User = Copy(Source.Data.Kick.User);
					Data.Kick.Host = Copy(Source.Data.Kick.Host);
					Data.Kick.Channel = Copy(Source.Data.Kick.Channel);
					Data.Kick.Victim = Copy(Source.Data.Kick.Victim);
					Data.Kick.Reason = Copy(Source.Data.Kick.Reason);
					break;

				default:
					break;
			}
		}

		~IRCMessage()
		{
			switch(Type)
			{
				case IRCMessageType::Welcome:
					Delete(Data.Welcome.Message);
					break;

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

				case IRCMessageType::Topic:
					Delete(Data.Topic.Channel);
					Delete(Data.Topic.Topic);
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

				case IRCMessageType::TopicChanged:
					Delete(Data.TopicChanged.Nick);
					Delete(Data.TopicChanged.User);
					Delete(Data.TopicChanged.Host);
					Delete(Data.TopicChanged.Channel);
					Delete(Data.TopicChanged.NewTopic);
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

		template<class Type>
		Type * Copy(Type * Source)
		{
			if(Source != NULL)
			{
				return new Type(*Source);
			}
			else
			{
				return NULL;
			}
		}
	};
}