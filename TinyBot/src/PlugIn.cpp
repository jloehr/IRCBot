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
#include "PlugIn.h"


//------------------------------------------//
//											//
//		  Konstructor & Destructor			//
//											//
//------------------------------------------//


CPlugIn::CPlugIn(const std::string & Filename)
	:m_Filename(Filename)
	,m_Initialized(false)
	,m_Handle(NULL), m_VersionFunction(NULL), m_CreateFunction(NULL), m_FreeFunction(NULL)
{


}

CPlugIn::~CPlugIn()
{
	Close();
}

//------------------------------------------//
//											//
//					Load					//
//											//
//------------------------------------------//

bool CPlugIn::Load()
{
	if(m_Filename == "")
	{
		return false;
	}

	if(!OpenLib())
	{
		return false;
	}

	if(!CheckVersion())
	{
		Close();
		return false;
	}

	if(!LoadSymbols())
	{
		Close();
		return false;
	}

	m_Initialized = true;

	return true;
}

bool CPlugIn::OpenLib()
{
	m_Handle = dlopen(m_Filename.c_str(), RTLD_LAZY);

	if(m_Handle == NULL)
	{
		Output::Error("Plugin Open: ", {m_Filename.c_str(), " -> ", dlerror()});
		return false;
	}

	return true;
}

bool CPlugIn::CheckVersion()
{
	m_VersionFunction = (PluginIntefaceVersionFunction) dlsym(m_Handle, PLUGIN_VERSION_FUNCTION);

	if(m_VersionFunction == NULL)
	{
		Output::Error("Plugin Check Version: ", {m_Filename.c_str(), " -> ", dlerror()});
		return false;
	}

	int PluginVersion = (*m_VersionFunction)();

	if(PluginVersion != PLUGIN_INTERFACE_VERSION)
	{
		Output::Error("Plugin Check Version: ", {m_Filename.c_str(), " -> Incorrect Version"});
		return false;
	}

	return true;
}

bool CPlugIn::LoadSymbols()
{
	//Load CreateInterface
	m_CreateFunction = (CreateServerInterfaceFunction) dlsym(m_Handle, PLUGIN_CREATE_INTERFACE_FUNCTION);

	if(m_CreateFunction == NULL)
	{
		Output::Error("Plugin: ", {m_Filename.c_str(), " -> Can't Load CreateInterface Function"});
		return false;
	}

	//Load CreateInterface
	m_FreeFunction = (FreeServerInterfaceFunction) dlsym(m_Handle, PLUGIN_FREE_INTERFACE_FUNCTION);

	if(m_FreeFunction == NULL)
	{
		Output::Error("Plugin: ", {m_Filename.c_str(), " -> Can't Load FreeInterface Function"});
		return false;
	}

	return true;
}

//------------------------------------------//
//											//
//					Close					//
//											//
//------------------------------------------//


void CPlugIn::Close()
{	
	if(m_Handle != NULL)
	{
		dlclose(m_Handle);

		m_Handle = NULL;
		m_VersionFunction = NULL;
		m_CreateFunction = NULL;
		m_FreeFunction = NULL;

		m_Initialized = false;
	}
}
	


//------------------------------------------//
//											//
//				Create & Free				//
//											//
//------------------------------------------//

IServerInterface * CPlugIn::CreateInterface(const std::string & ServerAdress, const std::string & Botname)
{
	if(m_Initialized)
	{
		return (m_CreateFunction)(ServerAdress, Botname);
	}
	else
	{
		return NULL;
	}
}

void CPlugIn::FreeInstance(IServerInterface * Instance)
{
	if(Instance != NULL)
	{
		(m_FreeFunction)(Instance);
	}
}