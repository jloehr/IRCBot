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
#include <dlfcn.h>

#include "Output.h"
#include "PlugInInterface.h"

class CPlugIn;
typedef std::vector<CPlugIn *> PluginVector;
typedef std::pair<IServerInterface *, CPlugIn *> PluginPair;
typedef std::vector<PluginPair> PluginPairVector; 
typedef std::vector<IChannelInterface *> ChannelPluginVector;

class CPlugIn
{
public:
	CPlugIn(const std::string & Filename);
	~CPlugIn();
	
	bool Load();
	IServerInterface * CreateInterface(const std::string & ServerAdress, const std::string & Botname);
	void FreeInstance(IServerInterface * Instance);

private:
	const std::string m_Filename;

	bool m_Initialized;

	void * m_Handle;
	PluginIntefaceVersionFunction m_VersionFunction;
	CreateServerInterfaceFunction m_CreateFunction;
	FreeServerInterfaceFunction m_FreeFunction;

	bool OpenLib();
	bool CheckVersion();
	bool LoadSymbols();

	void Close();

};