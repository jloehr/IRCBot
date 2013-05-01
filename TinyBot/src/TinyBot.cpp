#include "TinyBot.h"


//------------------------------------------//
//											//
//		  Konstructor & Destructor			//
//											//
//------------------------------------------//

CTinyBot::CTinyBot(const int WorkerThreadNum, const std::string Botname)
	:m_WorkerThreadNum(WorkerThreadNum), m_Botname(Botname)
	,m_ControlPanel(this)
{

}

CTinyBot::~CTinyBot()
{

}

//------------------------------------------//
//											//
//				Run & Stop					//
//											//
//------------------------------------------//

void CTinyBot::Run()
{
	std::cout << "Hello my name is " << m_Botname << ". I'm going to run with " << m_WorkerThreadNum << " + 1 Threads" << std::endl;

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