#pragma once

#include <string>

#include <tinyirc.h>

#include "Output.h"
#include "PlugInInterface.h"
#include "PlugIn.h"

class CServer;

class CChannel
{
public:
	CChannel(const std::string & Name, const std::string & Pass, const PluginPairVector & Plugins, CServer & ServerConnection);
	~CChannel();

	void Reset();

	void Join();

private:
	const std::string m_Name;
	const std::string m_Pass;

	CServer & m_ServerConnection;
	tinyirc::CParser & m_IRCParser;

	ChannelPluginVector m_Plugins;

};