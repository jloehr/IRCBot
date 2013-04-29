#include "TinyBot.h"


//------------------------------------------//
//											//
//		  Konstructor & Destructor			//
//											//
//------------------------------------------//

CTinyBot::CTinyBot(const int WorkerThreadNum, const std::string Botname)
	:m_WorkerThreadNum(WorkerThreadNum), m_Botname(Botname)
{

}

CTinyBot::~CTinyBot()
{

}

//------------------------------------------//
//											//
//			Run & Start Up					//
//											//
//------------------------------------------//

void CTinyBot::Run()
{
	std::cout << "Hello my name is " << m_Botname << ". I'm going to run with " << m_WorkerThreadNum << " + 1 Threads" << std::endl;
}
