#pragma once

#include <string>
#include <vector>

#define PLUGIN_INTERFACE_VERSION 1000
#define PLUGIN_VERSION_FUNCTION "GetPluginInterfaceVersion"
#define PLUGIN_CREATE_INTERFACE_FUNCTION "CreateServerInterface"
#define PLUGIN_FREE_INTERFACE_FUNCTION "FreeServerInterface"

class IChannelInterface;
class IServerInterface;

typedef int (*PluginIntefaceVersionFunction)();
typedef IServerInterface * (*CreateServerInterfaceFunction) (const std::string & ServerAdress, const std::string & Botname);
typedef void (*FreeServerInterfaceFunction)(IServerInterface * Instance);

class IServerInterface
{
public:
	virtual ~IServerInterface() {};

	virtual IChannelInterface & GetChannelInterface(const std::string & ChannelName) = 0;

	//Server Events
	virtual void OnWelcome		(const std::string & WelcomeMesage, std::string & Response) = 0;
	virtual void OnMOTD			(const std::string & MessageOfTheDay, std::string & Response) = 0;
	virtual void OnPrvtMessage	(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, std::string & Response) = 0;
	virtual void OnNotice		(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, std::string & Response) = 0;
	virtual void OnUserMode		(const std::string & Nick, const std::string & User, const std::string & Host, const char Mode, const bool Set, std::string & Response) = 0;

	virtual void OnShutdown		(std::string & Response) = 0;
	virtual void OnDisconnect	() = 0;

};

class IChannelInterface
{
public:
	virtual ~IChannelInterface() {};

	//Channel Events
	virtual void OnUserList	(const std::vector<std::string> & UserList, std::string & Response) = 0;
	virtual void OnTopic	(const std::string & Topic, std::string & Response) = 0;
	virtual void OnChannelMode	(const std::string & Nick, const std::string & User, const std::string & Host, const char Mode, const bool Set, const std::string & Param, std::string & Response) = 0;

	virtual void OnJoin		(const std::string & Nick, const std::string & User, const std::string & Host, std::string & Response) = 0;
	virtual void OnPart		(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, std::string & Response) = 0;
	virtual void OnQuit		(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, std::string & Response) = 0;
	virtual void OnKick		(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Victim, const std::string & Reason, std::string & Response) = 0;
	virtual void OnNickChange	(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & NewNick, std::string & Response) = 0;
	virtual void OnChangedTopic	(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & NewTopic, std::string & Response) = 0;

	virtual void OnNotice	(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Mesage, std::string & Response) = 0;
	virtual void OnMessage	(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Mesage, std::string & Response) = 0;
};