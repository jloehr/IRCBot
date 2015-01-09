/*
 *
 * Copyright (C) 2013 Julian Löhr
 * All rights reserved.
 *
 * This file is part of SQLLogPlugin, a Logging Plugin for TinyBot.
 * SQLLogPlugin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * SQLLogPlugin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with SQLLogPlugin.  If not, see <http://www.gnu.org/licenses/>.
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