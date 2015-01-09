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