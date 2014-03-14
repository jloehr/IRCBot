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
#include "TinyBotFactory.h"

const char * const CTinyBotFactory::Defaultname = "TinyBot";

CTinyBot * CTinyBotFactory::CreateTinyBot(const int argc, const char* argv[])
{
	CTinyBotFactory Fax(argc, argv);

	return Fax.Build();
}

//------------------------------------------//
//											//
//		  Konstructor & Destructor			//
//											//
//------------------------------------------//

CTinyBotFactory::CTinyBotFactory(const int argc, const char* argv[])
	:m_argc(argc), m_argv(argv)
	,m_Botname(CTinyBotFactory::Defaultname), m_ChannelVector(NULL)
	,m_Product(NULL)
{

}

CTinyBotFactory::~CTinyBotFactory()
{

}

//------------------------------------------//
//											//
//		  			Build					//
//											//
//------------------------------------------//

CTinyBot * CTinyBotFactory::Build()
{
	ParseArguments();

	m_Product = new CTinyBot(m_Botname, m_PluginPathVector);

	SetupServers();

	return m_Product;
}


void CTinyBotFactory::SetupServers()
{
	for(ServerDataStringVectorPairVector::iterator it = m_ServerVector.begin(); it != m_ServerVector.end(); ++it)
	{
		m_Product->Connect(*((*it)->first), *((*it)->second));
		CleanUp(*it);
		delete (*it);
	}

	m_ServerVector.clear();
}


//------------------------------------------//
//											//
//		  			Parsing					//
//											//
//------------------------------------------//


void CTinyBotFactory::ParseArguments()
{
	// -n string 	Botname
	//	server:port #channel,pass #channel,pass server2 #channel #channel,pass
	//				bsp.: irc.quakenet.org #foo,bar #help foo.randomirc.org:12345 #lobby
	//				joins channel foo with pass bar and channel help on irc.quakenet.org and also channel lobby on foo.randomirc.org with port
	// -l PathToPlugin1 -l PathtoPlugin2
	//				Load the Library at the given path as Plugin


	for (int i = 1; i < m_argc; ++i)
	{
		switch(m_argv[i][0])
		{

			//Channel
			case '#':
			case '&':
				
				if(m_ChannelVector == NULL)
				{
					std::cout << "Warning: Channel " << m_argv[i] << "will be ignored. Please check your command line paramenter" << std::endl;
				}
				else
				{
					const char * DelimiterPosition = strrchr(m_argv[i], ',');

					std::string * ChannelName = NULL;
					std::string * ChannelPass = NULL;

					if(DelimiterPosition == NULL)
					{
						ChannelName = new std::string(m_argv[i]);
						ChannelPass = new std::string();
					}
					else
					{
						ChannelName = new std::string(m_argv[i], (size_t)(DelimiterPosition - m_argv[i]));
						ChannelPass = new std::string(DelimiterPosition + 1);
					}
					
					m_ChannelVector->push_back(new StringPair(ChannelName, ChannelPass));
				}
				break;

			//Flag
			case '-':
				if(m_argv[i][2] == '\0')
				{
					switch(m_argv[i][1])
					{
					case 'n':
						m_Botname = m_argv[++i];
						break;
					case 'l':
						m_PluginPathVector.push_back(std::string(m_argv[++i]));
						break;
					}
					break;
				}

			//Server	
			default:
				{
					const char * PortDelemiterPosition = strchr(m_argv[i], ':');
					const char * PassDelemiterPosition = strchr(m_argv[i], ',');

					std::string * ServerAdress = NULL;
					std::string * ServerPort = NULL;
					std::string * ServerPass = NULL;

					if((PortDelemiterPosition == NULL) && (PassDelemiterPosition == NULL))
					{
						ServerAdress = new std::string(m_argv[i]);
						ServerPort = new std::string(DEFAULT_PORT);
						ServerPass = new std::string();
					} 
					else
					{
						const char *  ServerAdressEnd;

						if((PortDelemiterPosition != NULL) && (PassDelemiterPosition != NULL))
						{
							if(PortDelemiterPosition < PassDelemiterPosition)
							{
								ServerAdressEnd = PortDelemiterPosition;
								ServerPort = new std::string(PortDelemiterPosition + 1, (size_t)(PassDelemiterPosition - PortDelemiterPosition) - 1);
								ServerPass = new std::string(PassDelemiterPosition + 1);
							}
							else
							{
								ServerAdressEnd = PassDelemiterPosition;
								ServerPort = new std::string(PortDelemiterPosition + 1);
								ServerPass = new std::string(PassDelemiterPosition + 1, (size_t)(PortDelemiterPosition - PassDelemiterPosition) - 1);
							}

						}
						else if(PortDelemiterPosition != NULL)
						{
							ServerAdressEnd = PortDelemiterPosition;
							ServerPort = new std::string(PortDelemiterPosition + 1);
						}				
						else
						{
							ServerAdressEnd = PassDelemiterPosition;
							ServerPass = new std::string(PassDelemiterPosition + 1);
						}

						ServerAdress = new std::string(m_argv[i], (size_t)(ServerAdressEnd - m_argv[i]));
					}

					m_ChannelVector = new StringPairVector();

					ServerDataStringVectorPair * NewServer = new ServerDataStringVectorPair(new ServerData(ServerAdress, ServerPort, ServerPass), m_ChannelVector);

					m_ServerVector.push_back(NewServer);
				}
				break;
		}

	}
}

//------------------------------------------//
//											//
//		  			CleanUp					//
//											//
//------------------------------------------//


void CTinyBotFactory::CleanUp(ServerDataStringVectorPair * Entry)
{
	StringPair * Tmp;
	StringPairVector * TmpVector;
	ServerDataPointer TmpServerData;

	// Delete String Pair
	TmpServerData = Entry->first;
	Entry->first = NULL;

	TmpServerData->Flush();

	delete TmpServerData;

	TmpServerData = NULL;

	//delete Vector Content
	TmpVector = Entry->second;
	for(StringPairVector::iterator it = TmpVector->begin(); it != TmpVector->end(); ++it)
	{
		Tmp = (*it);
		
		delete Tmp->first;
		delete Tmp->second;
		delete Tmp;
	}
	
	delete TmpVector;

	Entry->second = NULL;
}