#pragma once

#include "PlugInInterface.h"


class CChannel: public IChannelInterface
{
public:
	CChannel(const std::string & ChannelName);
	virtual ~CChannel();

	//Channel Events
	virtual void OnUserList(const std::vector<std::string> & UserList, IResponseInterface & Response);
	virtual void OnTopic(const std::string & Topic, IResponseInterface & Response);
	virtual void OnChannelMode(const std::string & Nick, const std::string & User, const std::string & Host, const char Mode, const bool Set, const std::string & Param, IResponseInterface & Response);

	virtual void OnJoin(const std::string & Nick, const std::string & User, const std::string & Host, IResponseInterface & Response);
	virtual void OnPart(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response);
	virtual void OnQuit(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response);
	virtual void OnKick(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Victim, const std::string & Reason, IResponseInterface & Response);
	virtual void OnNickChange(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & NewNick, IResponseInterface & Response);
	virtual void OnChangedTopic(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & NewTopic, IResponseInterface & Response);

	virtual void OnNotice(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response);
	virtual void OnMessage(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response);

	void OnShutdown(IResponseInterface & Response);

private:
	const std::string & m_ChannelName;
};
