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
	m_ev_io_StdInWatcher.stop();
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

	if(revents & ev::ERROR)
	{
		Output::Error("ControlPanel", {"Unspecified Error signaled by Libev. Shutting down!"});
		m_Bot.Stop();
		return;
	}

	std::string InputBuffer;

	std::getline (std::cin, InputBuffer);

	if((InputBuffer.compare("exit") == 0) || (InputBuffer.compare("quit") == 0))
	{
		Output::Log({"Exiting..."});

		m_Bot.Stop();
	}
}