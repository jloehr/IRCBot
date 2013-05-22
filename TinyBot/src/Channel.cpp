#include "Channel.h"


//------------------------------------------//
//											//
//		  Konstructor & Destructor			//
//											//
//------------------------------------------//

CChannel::CChannel(const std::string * Name, const std::string * Pass)
	:m_Name(*Name), m_Pass(*Pass)
{

}

CChannel::~CChannel()
{

}