/*
 *
 * Copyright (C) 2013 Julian Löhr
 * All rights reserved.
 *
 * This file is part of TinyBot, an IRC Bot.
 * TinyBot is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
 * To read the full license, please check the LICENSE file.
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