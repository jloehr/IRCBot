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
#include <list>

#include <tinyirc.h>

#include "Output.h"
#include "PlugInInterface.h"
#include "PlugIn.h"
#include "ResponseWrapper.h"
#include "types.h"

class CServer;

class CChannel
{
public:
	CChannel(const std::string & Name, const std::string & Pass, const PluginPairVector & Plugins, CServer & ServerConnection);
	~CChannel();

	void Reset();

	bool UserIsIn(const std::string & Nick);

	void Join();

	void OnUserList	(const tinyirc::IRCMessage & Message);
	void OnTopic	(const tinyirc::IRCMessage & Message);
	void OnChannelMode	(const tinyirc::IRCMessage & Message);

	void OnJoin		(const tinyirc::IRCMessage & Message);
	void OnPart		(const tinyirc::IRCMessage & Message);
	void OnQuit		(const tinyirc::IRCMessage & Message);
	void OnKick		(const tinyirc::IRCMessage & Message);
	void OnNickChange	(const tinyirc::IRCMessage & Message);
	void OnChangedTopic	(const tinyirc::IRCMessage & Message);

	void OnNotice	(const tinyirc::IRCMessage & Message);
	void OnMessage	(const tinyirc::IRCMessage & Message);

	const std::string & GetChannelName();

private:
	const std::string m_Name;
	const std::string m_Pass;

	CServer & m_ServerConnection;
	tinyirc::CParser & m_IRCParser;

	ChannelPluginVector m_Plugins;
	StringSet m_NickList;

};

inline
const std::string & CChannel::GetChannelName()
{
	return m_Name;
}