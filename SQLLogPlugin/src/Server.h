#pragma once

#include <set>

#include "Channel.h"

typedef std::set<CChannel *> ChannelSet;

class CServer : public IServerInterface
{
public:
	CServer(const std::string & ServerAdress, const std::string & Botname);
	virtual ~CServer();

	virtual IChannelInterface & GetChannelInterface(const std::string & ChannelName);

	//Server Events
	virtual void OnWelcome		(const std::string & WelcomeMesage, IResponseInterface & Response);
	virtual void OnMOTD			(const std::string & MessageOfTheDay, IResponseInterface & Response);
	virtual void OnPrvtMessage	(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response);
	virtual void OnNotice		(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response);
	virtual void OnUserMode		(const std::string & Nick, const std::string & User, const std::string & Host, const char Mode, const bool Set, IResponseInterface & Response);

	virtual void OnShutdown		(IResponseInterface & Response);
	virtual void OnDisconnect	();

private:
	const std::string m_Nick;
	const std::string m_ServerAdress;

	ChannelSet m_Channels;

};