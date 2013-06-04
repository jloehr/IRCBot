#pragma once

#include <string>

#include <tinyirc.h>

#include "PlugInInterface.h"

class CServer;
class CChannel;

class CResponseWrapper : public IResponseInterface
{
public:
	CResponseWrapper(tinyirc::CParser & IRCParser, CServer & ServerConnection, CChannel * Channel = NULL);
	virtual ~CResponseWrapper();

	virtual void SendMessage(const std::string & Reciever, const std::string & Message);

	void Clear();
	void Send();

private:
	tinyirc::CParser & m_IRCParser;
	CServer & m_ServerConnection;
	CChannel * m_Channel;

	std::string m_MessageBuffer;
	tinyirc::IRCMessageVector m_MessageLoop;

	void LoopMessageBack(const std::string & Message);
};

inline
void CResponseWrapper::Clear()
{
	m_MessageBuffer.assign("");
	m_MessageLoop.clear();
}