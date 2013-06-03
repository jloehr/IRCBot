#include "ResponseWrapper.h"

//------------------------------------------//
//											//
//		  Konstructor & Destructor			//
//											//
//------------------------------------------//


CResponseWrapper::CResponseWrapper(tinyirc::CParser & IRCParser)
	:m_IRCParser(IRCParser)
{

}

CResponseWrapper::~CResponseWrapper()
{

}

//------------------------------------------//
//											//
//		  		Virtual Calls				//
//											//
//------------------------------------------//

void CResponseWrapper::SendMessage(const std::string & Reciever, const std::string & Message)
{
	m_IRCParser.SendMessage(Reciever, Message, m_MessageBuffer);
}