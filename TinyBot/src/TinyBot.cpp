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
	Output::Log({"Hello my name is ", m_Botname.c_str(), "."});

	Init();

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

	Output::Log({"Breaking Loop..."});
 }

//------------------------------------------//
//											//
//					Init					//
//											//
//------------------------------------------//

void CTinyBot::Init()
{
	m_ControlPanel.Init();

	for(ServerVector::iterator it = m_Servers.begin(); it != m_Servers.end(); ++it)
	{
		(*it)->Init();
	}
}

//------------------------------------------//
//											//
//				  Watchers					//
//											//
//------------------------------------------//

void CTinyBot::StartWatchers()
{
	m_ControlPanel.Start();

	for(ServerVector::iterator it = m_Servers.begin(); it != m_Servers.end(); ++it)
	{
		(*it)->Start();
	}
	
}

void CTinyBot::StopWatchers()
{
	m_ControlPanel.Stop();

	for(ServerVector::iterator it = m_Servers.begin(); it != m_Servers.end(); ++it)
	{
		(*it)->Stop();
	}
}

//------------------------------------------//
//											//
//				Connect						//
//											//
//------------------------------------------//

void CTinyBot::Connect(StringPair * Server, StringPairVector * Channels)
{
	m_Servers.push_back(new CServer(&m_Botname, Server->first, Server->second, Channels));
}