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