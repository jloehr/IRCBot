#include "TinyBot.h"


//------------------------------------------//
//											//
//		  Konstructor & Destructor			//
//											//
//------------------------------------------//

CTinyBot::CTinyBot(const std::string Botname)
	:m_Botname(Botname)
	,m_ControlPanel(this)
{

}

CTinyBot::~CTinyBot()
{
	for(ServerVector::iterator it = m_Servers.begin(); it != m_Servers.end(); ++it)
	{
		delete (*it);
	}
	
	m_Servers.clear();
}

//------------------------------------------//
//											//
//				Run & Stop					//
//											//
//------------------------------------------//

void CTinyBot::Run()
{
	std::cout << "Hello my name is " << m_Botname << "." << std::endl;

	m_ControlPanel.Init();

	StartWatchers();

	RunLoop();

	StopWatchers();
}

void CTinyBot::RunLoop()
{
	struct ev_loop * loop = ev_default_loop(0);

	ev_run(loop, 0);
}

void CTinyBot::Stop()
 {
	ev_break (EV_DEFAULT_ EVBREAK_ALL);

	std::cout << "Breaking Loop...\n" << std::endl;
 }


//------------------------------------------//
//											//
//				Connect						//
//											//
//------------------------------------------//

void CTinyBot::Connect(StringPair * Server, StringPairVector * Channels)
{
	std::string Port = (Server->second == NULL) ? "-" : (*(Server->second));
	std::cout << "Connect to " << (*(Server->first)) << "(:" << Port << ") joining channels(" << Channels->size() << "):" << std::endl;
	
	for(StringPairVector::iterator it = Channels->begin(); it != Channels->end(); ++it)
	{
		std::string Password = ((*it)->second == NULL) ? "-" : (*((*it)->second));
		std::cout << (*((*it)->first)) << " , " << Password << std::endl;


	}

	std::cout << std::endl;
	
	m_Servers.push_back(new CServer(&m_Botname, Server->first, Server->second, Channels));

}


//------------------------------------------//
//											//
//				Watchers					//
//											//
//------------------------------------------//

void CTinyBot::StartWatchers()
{
	m_ControlPanel.Start();
}

void CTinyBot::StopWatchers()
{
	m_ControlPanel.Stop();
}