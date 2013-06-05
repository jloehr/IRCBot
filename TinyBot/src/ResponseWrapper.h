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
#pragma once

#include <string>

#include <tinyirc.h>

#include "PlugInInterface.h"

class CServer;
class CChannel;

class CResponseWrapper : public IResponseInterface
{
public:
	CResponseWrapper(tinyirc::CParser & IRCParser, CServer & ServerConnection, CChannel * Channel = NULL);
	virtual ~CResponseWrapper();

	virtual void SendMessage(const std::string & Reciever, const std::string & Message);

	void Clear();
	void Send();

private:
	tinyirc::CParser & m_IRCParser;
	CServer & m_ServerConnection;
	CChannel * m_Channel;

	std::string m_MessageBuffer;
	tinyirc::IRCMessageVector m_MessageLoop;

	void LoopMessageBack(const std::string & Message);
};

inline
void CResponseWrapper::Clear()
{
	m_MessageBuffer.assign("");
	m_MessageLoop.clear();
}