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
	for(StringPairStringVectorPairVector::iterator it = m_ServerVector.begin(); it != m_ServerVector.end(); ++it)
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
					const char * DelimiterPosition = strrchr(m_argv[i], ':');

					std::string * ServerAdress = NULL;
					std::string * ServerPort = NULL;
					
					if(DelimiterPosition == NULL)
					{
						ServerAdress = new std::string(m_argv[i]);
						ServerPort = new std::string(DEFAULT_PORT);
					}
					else
					{

						ServerAdress = new std::string(m_argv[i], (size_t)(DelimiterPosition - m_argv[i]));
						ServerPort = new std::string(DelimiterPosition + 1);
					}

					m_ChannelVector = new StringPairVector();

					StringPairStringVectorPair * NewServer = new StringPairStringVectorPair(new StringPair(ServerAdress, ServerPort), m_ChannelVector);


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


void CTinyBotFactory::CleanUp(StringPairStringVectorPair * Entry)
{
	StringPair * Tmp;
	StringPairVector * TmpVector;

	// Delete String Pair
	Tmp = Entry->first;
	Entry->first = NULL;

	delete Tmp->first;
	delete Tmp->second;

	delete Tmp;

	Tmp = NULL;

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