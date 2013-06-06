/*
 *
 * Copyright (C) 2013 Julian Löhr
 * All rights reserved.
 *
 * This file is part of SQLLogPlugin, a Logging Plugin for TinyBot.
 * SQLLogPlugin is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
 * To read the full license, please check the LICENSE file.
 *
*/
#include "PlugInInterface.h"
#include "Server.h"

extern "C"
{
	int GetPluginInterfaceVersion()
	{
		return PLUGIN_INTERFACE_VERSION;
	}

	IServerInterface * CreateServerInterface(const std::string & ServerAdress, const std::string & Botname)
	{
		return new CServer(ServerAdress, Botname);
	}

	void FreeServerInterface (IServerInterface * Instance)
	{
		if(Instance != NULL)
		{
			delete Instance;
		}
	}
}