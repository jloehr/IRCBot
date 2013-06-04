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