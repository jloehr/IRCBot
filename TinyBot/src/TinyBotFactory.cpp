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
	:m_argc(argc), m_argv(argv), m_WorkerThreadNum(1), m_Botname(CTinyBotFactory::Defaultname), m_Product(NULL)
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

	m_Product = new CTinyBot(m_WorkerThreadNum, m_Botname);

	return m_Product;
}

//------------------------------------------//
//											//
//		  			Parsing					//
//											//
//------------------------------------------//

void CTinyBotFactory::ParseArguments()
{
	// -t int		Workerthreads
	// -n string 	Botname
	//	server:port #channel,pass #channel,pass server2 #channel #channel,pass
	//				bsp.: irc.quakenet.org #foo,bar #help foo.randomirc.org:12345 #lobby
	//				joins channel foo with pass bar and channel help on irc.quakenet.org and also channel lobby on foo.randomirc.org with port

	for (int i = 0; i < m_argc; ++i)
	{
		if((m_argv[i][0] == '-') && (m_argv[i][2] == '\0'))
			{
				switch(m_argv[i][1])
				{
				case 't':
					m_WorkerThreadNum = atoi(m_argv[++i]);
					break;
				case 'n':
					m_Botname = m_argv[++i];
					break;
				}
			}
	}

	if(m_WorkerThreadNum < 1)
	{
		m_WorkerThreadNum = 1;
	}
}