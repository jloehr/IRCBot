/*
 *
 * Copyright (C) 2013 Julian Löhr
 * All rights reserved.
 *
 * This file is part of TinyBot, an IRC Bot.
 * TinyBot is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * TinyBot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with TinyBot.  If not, see <http://www.gnu.org/licenses/>.
 *
*/
#include "ResponseWrapper.h"
#include "Server.h"
#include "Channel.h"

//------------------------------------------//
//											//
//		  Konstructor & Destructor			//
//											//
//------------------------------------------//


CResponseWrapper::CResponseWrapper(tinyirc::CParser & IRCParser, CServer & ServerConnection, CChannel * Channel)
	:m_IRCParser(IRCParser)
	,m_ServerConnection(ServerConnection)
	,m_Channel(Channel)
{

}

CResponseWrapper::~CResponseWrapper()
{

}

//------------------------------------------//
//											//
//		  		Send Message				//
//											//
//------------------------------------------//

void CResponseWrapper::SendMessage(const std::string & Reciever, const std::string & Message)
{
	m_IRCParser.SendMessage(Reciever, Message, m_MessageBuffer);

	if((m_Channel != NULL) && (Reciever == m_Channel->GetChannelName()))
	{
		LoopMessageBack(Message);
	}	
}

void CResponseWrapper::LoopMessageBack(const std::string & Message)
{
	tinyirc::IRCMessage Tmp;

	Tmp.Type = tinyirc::IRCMessageType::Message;
	Tmp.Data.Message.Nick = new std::string(m_ServerConnection.GetBotname());
	Tmp.Data.Message.User = new std::string("");
	Tmp.Data.Message.Host = new std::string("");
	Tmp.Data.Message.Reciever = new std::string("");
	Tmp.Data.Message.Message = new std::string(Message);

	m_MessageLoop.push_back(Tmp);
}


//------------------------------------------//
//											//
//		  			Send					//
//											//
//------------------------------------------//

void CResponseWrapper::Send()
{
	if(m_MessageBuffer.size() > 0)
	{
		m_ServerConnection.Send(m_MessageBuffer);

		if(m_Channel != NULL)
		{
			for(tinyirc::IRCMessageVector::iterator it = m_MessageLoop.begin(); it != m_MessageLoop.end(); ++it)
			{
				m_Channel->OnMessage(*it);	
			}	
		}
	}

	Clear();
}