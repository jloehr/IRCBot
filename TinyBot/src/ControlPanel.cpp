#include "ControlPanel.h"
#include "TinyBot.h"

//------------------------------------------//
//											//
//		  Konstructor & Destructor			//
//											//
//------------------------------------------//

CControlPanel::CControlPanel(CTinyBot * Bot)
	:m_Bot(*Bot)
{
	m_ev_io_StdInWatcher.set<CControlPanel, &CControlPanel::io_cb_StdIn> (this);

}

CControlPanel::~CControlPanel()
{

}

//------------------------------------------//
//											//
//			Init, Start & Stop				//
//											//
//------------------------------------------//

void CControlPanel::Init()
{
	m_ev_io_StdInWatcher.set(STDIN_FILENO ,ev::READ);
}

void CControlPanel::Start()
{
	m_ev_io_StdInWatcher.start();
}

void CControlPanel::Stop()
{
	m_ev_io_StdInWatcher.stop();
}

//------------------------------------------//
//											//
//				Callback					//
//											//
//------------------------------------------//
	
void CControlPanel::io_cb_StdIn(ev::io   &w, int revents)
{
	std::string InputBuffer;

	std::getline (std::cin, InputBuffer);

	if(InputBuffer.compare("exit") || InputBuffer.compare("quit"))
	{
		std::cout << "Exiting...\n" << std::endl;

		m_Bot.Stop();
	}
}