#pragma once

#include <string>

#include <tinyirc.h>

#include "PlugInInterface.h"

class CResponseWrapper : public IResponseInterface
{
public:
	CResponseWrapper(tinyirc::CParser & IRCParser);
	virtual ~CResponseWrapper();

	virtual void SendMessage(const std::string & Reciever, const std::string & Message);

	bool Filled();
	std::string & GetResponse();
	void Clear();

private:
	tinyirc::CParser & m_IRCParser;
	std::string m_MessageBuffer;

};

inline
bool CResponseWrapper::Filled()
{
	return (!m_MessageBuffer.empty());
}

inline
std::string & CResponseWrapper::GetResponse()
{
	return m_MessageBuffer;
}

inline
void CResponseWrapper::Clear()
{
	m_MessageBuffer.assign("");
}