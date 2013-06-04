#include "Channel.h"

const std::string CChannel::HelpCommand("Help");
const std::string CChannel::LogCommand("Log [Enable|Disable]");
const std::string CChannel::PrintLogCommand("PrintLog");
const std::string CChannel::LastSeenCommand("LastSeen NICKNAME { NICKNAME }");

const CChannel::CommandFunctionMap CChannel::m_CommandFunctionMap = {
																		{"Log", &CChannel::CommandLog },
																		{"log", &CChannel::CommandLog },

																		{"printlog", &CChannel::CommandPrint },
																		{"Printlog", &CChannel::CommandPrint },
																		{"PrintLog", &CChannel::CommandPrint },
																		{"printLog", &CChannel::CommandPrint },

																		{"lastseen", &CChannel::CommandLastSeen },
																		{"LastSeen", &CChannel::CommandLastSeen },
																		{"Lastseen", &CChannel::CommandLastSeen },
																		{"LastSeen", &CChannel::CommandLastSeen },

																		{"Help", &CChannel::CommandHelp },
																		{"help", &CChannel::CommandHelp }
																	};

//------------------------------------------//
//											//
//		  Konstructor & Destructor			//
//											//
//------------------------------------------//

CChannel::CChannel(const std::string & BotName, const std::string & ChannelName)
	:m_BotName(BotName), m_ChannelName(ChannelName), m_CommandPrefix(BotName + ": ")
	,m_LogEnabled(true)
{

}

CChannel::~CChannel()
{
	LogAndClearUserList();
}

//------------------------------------------//
//											//
//				   Calls					//
//											//
//------------------------------------------//

void CChannel::OnUserList(const std::vector<std::string> & UserList, IResponseInterface & Response)
{
	for(std::vector<std::string>::const_iterator it = UserList.begin(); it != UserList.end(); ++it)
	{
		m_NickList.insert(*it);
	}

	CDatabaseWrapper::Singleton.LogMessage(m_ChannelName, " --- Bot joined Channel " + m_ChannelName + " as " + m_BotName + " --- ");	
	
}

void CChannel::OnTopic(const std::string & Topic, IResponseInterface & Response)
{

}

void CChannel::OnChannelMode(const std::string & Nick, const std::string & User, const std::string & Host, const char Mode, const bool Set, const std::string & Param, IResponseInterface & Response)
{
	if(m_LogEnabled)
	{
		std::string SetSymbol = Set ? "+" : "-";
		std::string LogMessage("mode/" + m_ChannelName + " [" + SetSymbol + Mode + " " + Param + "] by " + Nick);
		CDatabaseWrapper::Singleton.LogMessage(m_ChannelName, LogMessage);
	}
}

void CChannel::OnJoin(const std::string & Nick, const std::string & User, const std::string & Host, IResponseInterface & Response)
{
	m_NickList.insert(Nick);

	if(m_LogEnabled)
	{
		std::string LogMessage(Nick + " [" + User + "@"+ Host + "] has joined " + m_ChannelName);
		CDatabaseWrapper::Singleton.LogMessage(m_ChannelName, LogMessage);
	}
}

void CChannel::OnPart(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response)
{
	m_NickList.erase(Nick);

	if(m_LogEnabled)
	{
		std::string LogMessage(Nick + " [" + User + "@"+ Host + "] has left " + m_ChannelName + " [" + Message + "]");
		CDatabaseWrapper::Singleton.LogMessage(m_ChannelName, LogMessage);
		CDatabaseWrapper::Singleton.UserLeft(m_ChannelName, Nick, "[Part]" + Message);
	}
}

void CChannel::OnQuit(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response)
{
	m_NickList.erase(Nick);

	if(m_LogEnabled)
	{
		std::string LogMessage(Nick + " [" + User + "@"+ Host + "] has quit " + m_ChannelName + " [" + Message + "]");
		CDatabaseWrapper::Singleton.LogMessage(m_ChannelName, LogMessage);
		CDatabaseWrapper::Singleton.UserLeft(m_ChannelName, Nick, "[Quit]" + Message);
	}
}

void CChannel::OnKick(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Victim, const std::string & Reason, IResponseInterface & Response)
{
	m_NickList.erase(Victim);

	if(m_LogEnabled)
	{
		std::string LogMessage(Victim + " was kicked from " + m_ChannelName + " by " + Nick + " [" + Reason + "]");
		CDatabaseWrapper::Singleton.LogMessage(m_ChannelName, LogMessage);
		CDatabaseWrapper::Singleton.UserLeft(m_ChannelName, Victim, "[Kicked by " + Nick + "]" + Reason);
	}
}

void CChannel::OnNickChange(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & NewNick, IResponseInterface & Response)
{
	m_NickList.erase(Nick);
	m_NickList.insert(NewNick);

	if(m_LogEnabled)
	{
		std::string LogMessage(Nick + " has changed his nick to " + NewNick);
		CDatabaseWrapper::Singleton.LogMessage(m_ChannelName, LogMessage);
		CDatabaseWrapper::Singleton.UserLeft(m_ChannelName, Nick, "[NickChange] New Nick " + NewNick);
	}
}

void CChannel::OnChangedTopic(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & NewTopic, IResponseInterface & Response)
{
	if(m_LogEnabled)
	{
		std::string LogMessage(Nick + " changed the topic of " + m_ChannelName + " to: " + NewTopic);
		CDatabaseWrapper::Singleton.LogMessage(m_ChannelName, LogMessage);	
	}
}

void CChannel::OnNotice(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response)
{

}

void CChannel::OnMessage(const std::string & Nick, const std::string & User, const std::string & Host, const std::string & Message, IResponseInterface & Response)
{
	if(m_LogEnabled)
	{
		std::string LogMessage("<" + Nick + "> " + Message);
		CDatabaseWrapper::Singleton.LogMessage(m_ChannelName, LogMessage);
	}

	if(Message.substr(0, m_CommandPrefix.size()) == m_CommandPrefix)
	{
		std::string Command;
		StringVector Parameter;

		ParseCommand(Message.substr(m_CommandPrefix.size()), Command, Parameter);

		CommandFunctionMap::const_iterator Entry = m_CommandFunctionMap.find(Command);

		if(Entry != m_CommandFunctionMap.end())
		{
			(this->*(Entry->second))(Parameter, Response);
		}
		else
		{
			//Send Commands
			Response.SendMessage(m_ChannelName, std::string("Invalid Command! Please use one of the following:"));
			PrintCommands(Response);
		}

	}
}

void CChannel::OnShutdown(IResponseInterface & Response)
{
	CDatabaseWrapper::Singleton.LogMessage(m_ChannelName, std::string(" --- The Bot is shutting down --- "));
	LogAndClearUserList();
}

void CChannel::OnDisconnect()
{
	CDatabaseWrapper::Singleton.LogMessage(m_ChannelName, std::string(" --- Connection Lost --- "));
	LogAndClearUserList();
}


//------------------------------------------//
//											//
//			   Command Parsing				//
//											//
//------------------------------------------//

void CChannel::ParseCommand(const std::string & MessageTail, std::string & Command, StringVector & Parameter)
{
	size_t ParsePosition = 0;
	size_t SpacePosition = MessageTail.find(' ');

	Command = MessageTail.substr(ParsePosition, SpacePosition);

	while(SpacePosition != std::string::npos)
	{
		ParsePosition = SpacePosition + 1;
		SpacePosition = MessageTail.find(' ', ParsePosition);
		Parameter.push_back(MessageTail.substr(ParsePosition, SpacePosition - ParsePosition));
	}
}

//------------------------------------------//
//											//
//			 Command Processing				//
//											//
//------------------------------------------//

void CChannel::CommandLog(const StringVector & Parameter, IResponseInterface & Response)
{
	if(Parameter.empty())
	{
		std::string Status = m_LogEnabled ? "enabled" : "disabled";
		Response.SendMessage(m_ChannelName, "Logging is currently " + Status);
		return;
	}

	if(Parameter.size() > 1)
	{
		Response.SendMessage(m_ChannelName, std::string("Too much Parameters! Use the Command as follows:"));
		Response.SendMessage(m_ChannelName, CChannel::LogCommand);
		return;
	}

	if((Parameter[0] == "Enable") || (Parameter[0] == "enable"))
	{
		if(m_LogEnabled)
		{
			Response.SendMessage(m_ChannelName, std::string("Logging is already enabled"));
		}
		else
		{
			m_LogEnabled = true;
			Response.SendMessage(m_ChannelName, std::string(" --- Logging has been enabled --- "));
		}

		return;
	}

	if((Parameter[0] == "Disable") || (Parameter[0] == "disable"))
	{
		if(m_LogEnabled)
		{
			LogUserList(std::string("Logging disabled"));
			m_LogEnabled = false;
			CDatabaseWrapper::Singleton.LogMessage(m_ChannelName, std::string(" --- Logging has been disabled --- "));
			Response.SendMessage(m_ChannelName, std::string(" --- Logging has been disabled --- "));
		}
		else
		{
			Response.SendMessage(m_ChannelName, std::string("Logging is already disabled"));
		}

		return;
	}

	//One Parameter, which is not enable nor disable
	Response.SendMessage(m_ChannelName, std::string("Invalid Parameter! Use the Command as follows:"));
	Response.SendMessage(m_ChannelName, CChannel::LogCommand);

}

void CChannel::CommandPrint(const StringVector & Parameter, IResponseInterface & Response)
{
	Response.SendMessage(m_ChannelName, std::string("Pront"));
}

void CChannel::CommandLastSeen(const StringVector & Parameter, IResponseInterface & Response)
{
	bool NameGiven = false;

	if(!m_LogEnabled)
	{
		Response.SendMessage(m_ChannelName, std::string("Be adviced that logging is currently disabled!"));
	}

	for(StringVector::const_iterator it = Parameter.begin(); it != Parameter.end(); ++it)
	{
		if(!(*it).empty())
		{
			NameGiven = true;

			if(m_NickList.find((*it)) != m_NickList.end())
			{
				Response.SendMessage(m_ChannelName, std::string((*it) + " is currently present"));
			}
			else
			{
				std::string DateTime, Reason;
				CDatabaseWrapper::Singleton.LastSeen(m_ChannelName, (*it), DateTime, Reason);

				if(DateTime.empty())
				{
					Response.SendMessage(m_ChannelName, std::string((*it) + " is unkown"));
				}
				else
				{
					Response.SendMessage(m_ChannelName, std::string((*it) + " is absent since " + DateTime + " " + Reason));
				}
			}
		}
	}

	if(!NameGiven)
	{
		Response.SendMessage(m_ChannelName, std::string("No Name given! Use the Command as follows:"));
		Response.SendMessage(m_ChannelName, CChannel::LastSeenCommand);
		return;
	}
}

void CChannel::CommandHelp(const StringVector & Parameter, IResponseInterface & Response)
{

	Response.SendMessage(m_ChannelName, std::string("I know the following commands:"));
	PrintCommands(Response);
}

void CChannel::PrintCommands(IResponseInterface & Response)
{
	Response.SendMessage(m_ChannelName, CChannel::HelpCommand);
	Response.SendMessage(m_ChannelName, CChannel::LogCommand);
	Response.SendMessage(m_ChannelName, CChannel::PrintLogCommand);
	Response.SendMessage(m_ChannelName, CChannel::LastSeenCommand);
}

//------------------------------------------//
//											//
//				 User List					//
//											//
//------------------------------------------//

void CChannel::LogAndClearUserList()
{
	LogUserList(std::string("Bot was shutdown"));
	m_NickList.clear();
}


void CChannel::LogUserList(const std::string & Reason)
{
	for(StringSet::iterator it = m_NickList.begin(); it != m_NickList.end(); ++it)
	{		
		CDatabaseWrapper::Singleton.UserLeft(m_ChannelName, (*it), Reason);		
	}

}



