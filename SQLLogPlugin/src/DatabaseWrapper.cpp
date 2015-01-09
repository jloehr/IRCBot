/*
 *
 * Copyright (C) 2013 Julian Löhr
 * All rights reserved.
 *
 * This file is part of SQLLogPlugin, a Logging Plugin for TinyBot.
 * SQLLogPlugin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * SQLLogPlugin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with SQLLogPlugin.  If not, see <http://www.gnu.org/licenses/>.
 *
*/
#include "DatabaseWrapper.h"

const std::string CDatabaseWrapper::LogTableName = std::string("chatlog");
const std::string CDatabaseWrapper::LastSeenTableName = std::string("lastseen");
const std::string CDatabaseWrapper::PrepareQuery = std::string("CREATE TABLE IF NOT EXISTS "+CDatabaseWrapper::LogTableName+" (Time INTEGER, Channel TEXT, Message TEXT); CREATE TABLE IF NOT EXISTS "+CDatabaseWrapper::LastSeenTableName+" (Time INTEGER, Channel TEXT, Nick TEXT, Reason TEXT, PRIMARY KEY (Nick, Channel));");
const std::string CDatabaseWrapper::InsertLogQuery = std::string("INSERT INTO "+CDatabaseWrapper::LogTableName+" VALUES (strftime('%s','now'), ?, ?);");
const std::string CDatabaseWrapper::GetLogQuery = std::string("SELECT strftime('%d.%m.%Y %H:%M', Time, 'unixepoch', 'localtime') AS TimeStamp, Message FROM "+CDatabaseWrapper::LogTableName+" WHERE Channel = ? ORDER BY rowid DESC LIMIT ? OFFSET ?;");
const std::string CDatabaseWrapper::InsertLastSeenQuery = std::string("INSERT OR REPLACE INTO "+CDatabaseWrapper::LastSeenTableName+" VALUES(strftime('%s','now'), ?, ?, ?);");
const std::string CDatabaseWrapper::GetLastSeenQuery = std::string("SELECT strftime('%d.%m.%Y %H:%M', Time, 'unixepoch', 'localtime') AS TimeStamp, Reason FROM "+CDatabaseWrapper::LastSeenTableName+" WHERE (Channel = ?) AND (Nick = ?) ORDER BY Time ASC;");

CDatabaseWrapper CDatabaseWrapper::Singleton = CDatabaseWrapper();

//------------------------------------------//
//											//
//		  Konstructor & Destructor			//
//											//
//------------------------------------------//

CDatabaseWrapper::CDatabaseWrapper()
	:m_dbHandle(NULL)
	,m_InsertLogStatement(NULL)
	,m_GetLogStatement(NULL)
	,m_InsertLastSeenStatement(NULL)
	,m_GetLastSeenStatement(NULL)
{
	OpenDB();
	PrepareDatabase();
	PrepareStatements();
}

CDatabaseWrapper::~CDatabaseWrapper()
{
	CloseDB();
}

//------------------------------------------//
//											//
//		 		Open & Prepare				//
//											//
//------------------------------------------//

void CDatabaseWrapper::OpenDB()
{
	int Result = sqlite3_open("IRCLog.sqlite", &m_dbHandle);

	if(Result != SQLITE_OK)
	{
		Output::Error("SQLLite Plugin", { "On opening the DB -> ", sqlite3_errmsg(m_dbHandle) });
		sqlite3_close(m_dbHandle);
		m_dbHandle = NULL;
	}
}

void CDatabaseWrapper::PrepareDatabase()
{
	if(!Connected())
	{
		return;
	}

	char * ErrorMsg;
	int Result = sqlite3_exec(m_dbHandle, CDatabaseWrapper::PrepareQuery.c_str(), NULL, NULL, &ErrorMsg);

	if(Result != 0)
	{
		Output::Error("SQLLite Plugin", { "On executin the initial statement -> ", ErrorMsg});
		sqlite3_free(ErrorMsg);
		CloseDB();
		return;
	}
}

void CDatabaseWrapper::PrepareStatements()
{	
	if(!Connected())
	{
		return;
	}

	int Result = 0;

	//Insert Log
	Result = sqlite3_prepare_v2(m_dbHandle, CDatabaseWrapper::InsertLogQuery.c_str(), CDatabaseWrapper::InsertLogQuery.size(), &m_InsertLogStatement, NULL);
	if(Result != SQLITE_OK)
	{
		Output::Error("SQLLite Plugin", { "On preparing the InsertLog Statement -> ", sqlite3_errmsg(m_dbHandle) });
		CloseDB();
		return;
	}

	//Get Log
	Result = sqlite3_prepare_v2(m_dbHandle, CDatabaseWrapper::GetLogQuery.c_str(), CDatabaseWrapper::GetLogQuery.size(), &m_GetLogStatement, NULL);
	if(Result != SQLITE_OK)
	{
		Output::Error("SQLLite Plugin", { "On preparing the GetLog Statement -> ", sqlite3_errmsg(m_dbHandle) });
		CloseDB();
		return;
	}
	
	//Insert LastSeen
	Result = sqlite3_prepare_v2(m_dbHandle, CDatabaseWrapper::InsertLastSeenQuery.c_str(), CDatabaseWrapper::InsertLastSeenQuery.size(), &m_InsertLastSeenStatement, NULL);
	if(Result != SQLITE_OK)
	{
		Output::Error("SQLLite Plugin", { "On preparing the InsertLastSeen Statement -> ", sqlite3_errmsg(m_dbHandle) });
		CloseDB();
		return;
	}

	//Get LastSeen
	Result = sqlite3_prepare_v2(m_dbHandle, CDatabaseWrapper::GetLastSeenQuery.c_str(), CDatabaseWrapper::GetLastSeenQuery.size(), &m_GetLastSeenStatement, NULL);
	if(Result != SQLITE_OK)
	{
		Output::Error("SQLLite Plugin", { "On preparing the GetLastSeen Statement -> ", sqlite3_errmsg(m_dbHandle) });
		CloseDB();
		return;
	}
}

//------------------------------------------//
//											//
//		 	Finalize & Close				//
//											//
//------------------------------------------//


void CDatabaseWrapper::CloseDB()
{
	FinalizeStatements();

	int Result = sqlite3_close(m_dbHandle);
	m_dbHandle = NULL;

	if(Result != SQLITE_OK)
	{
		//Error
		Output::Error("SQLLite Plugin", { "On closing the DB -> ", sqlite3_errmsg(m_dbHandle) });
	}
}

void CDatabaseWrapper::FinalizeStatements()
{
	sqlite3_finalize(m_InsertLogStatement);
	sqlite3_finalize(m_GetLogStatement);
	sqlite3_finalize(m_InsertLastSeenStatement);
	sqlite3_finalize(m_GetLastSeenStatement);

	m_InsertLogStatement = NULL;
	m_GetLogStatement = NULL;
	m_InsertLastSeenStatement = NULL;
	m_GetLastSeenStatement = NULL;
}



//------------------------------------------//
//											//
//		 	Finalize & Close				//
//											//
//------------------------------------------//

void CDatabaseWrapper::LogMessage(const std::string & Channel, const std::string & Message)
{
	if(!Connected())
	{
		return;
	}

	if((Channel.size() <= 0) || (Message.size() <= 0))
	{
		return;
	}

	int Result = 0;
	sqlite3_reset(m_InsertLogStatement);
	sqlite3_clear_bindings(m_InsertLogStatement);

	Result = sqlite3_bind_text(m_InsertLogStatement, 1, Channel.c_str(), Channel.size(), SQLITE_STATIC);
	if(Result != SQLITE_OK)
	{
		Output::Error("SQLLite Plugin", { "Bind Channel for Log Message -> ", sqlite3_errmsg(m_dbHandle) });
		return;
	}

	Result = sqlite3_bind_text(m_InsertLogStatement, 2, Message.c_str(), Message.size(), SQLITE_STATIC);
	if(Result != SQLITE_OK)
	{
		Output::Error("SQLLite Plugin", { "Bind Message for Log Message -> ", sqlite3_errmsg(m_dbHandle) });
		return;
	}

	while((Result = sqlite3_step(m_InsertLogStatement))  != SQLITE_DONE)
	{
		if(Result != SQLITE_BUSY)
		{
			Output::Error("SQLLite Plugin", { "Executing the Insert Log -> ", sqlite3_errmsg(m_dbHandle) });
			return;
		}		
	}
}

void CDatabaseWrapper::GetLog(const std::string & Channel, const int limit, const int offset, StringPairVector & OutVector)
{

	if(!Connected())
	{
		return;
	}

	if(Channel.size() <= 0)
	{
		return;
	}

	int Result = 0;
	sqlite3_reset(m_GetLogStatement);
	sqlite3_clear_bindings(m_GetLogStatement);

	Result = sqlite3_bind_text(m_GetLogStatement, 1, Channel.c_str(), Channel.size(), SQLITE_STATIC);
	if(Result != SQLITE_OK)
	{
		Output::Error("SQLLite Plugin", { "Bind Channel for Get Log -> ", sqlite3_errmsg(m_dbHandle) });
		return;
	}

	Result = sqlite3_bind_int(m_GetLogStatement, 2, limit);
	if(Result != SQLITE_OK)
	{
		Output::Error("SQLLite Plugin", { "Bind Limit for Get Log -> ", sqlite3_errmsg(m_dbHandle) });
		return;
	}

	Result = sqlite3_bind_int(m_GetLogStatement, 3, offset);
	if(Result != SQLITE_OK)
	{
		Output::Error("SQLLite Plugin", { "Bind Offset for Get Log -> ", sqlite3_errmsg(m_dbHandle) });
		return;
	}

	while((Result = sqlite3_step(m_GetLogStatement)) != SQLITE_DONE)
	{
		if(Result == SQLITE_BUSY)
		{
			continue;
		}

		if(Result == SQLITE_ROW)
		{
			OutVector.push_back(StringPair(std::string(reinterpret_cast<const char*>(sqlite3_column_text(m_GetLogStatement, 0))), std::string(reinterpret_cast<const char*>(sqlite3_column_text(m_GetLogStatement, 1)))));
		}
		else
		{
			OutVector.clear();
			Output::Error("SQLLite Plugin", { "Executing the Get Log -> ", sqlite3_errmsg(m_dbHandle) });
			break;
		}
	}

}


//------------------------------------------//
//											//
//		 	Finalize & Close				//
//											//
//------------------------------------------//

void CDatabaseWrapper::UserLeft(const std::string & Channel, const std::string & Nick, const std::string & Reason)
{
	if(!Connected())
	{
		return;
	}

	if((Channel.size() <= 0) || (Nick.size() <= 0))
	{
		return;
	}

	int Result = 0;
	sqlite3_reset(m_InsertLastSeenStatement);
	sqlite3_clear_bindings(m_InsertLastSeenStatement);

	Result = sqlite3_bind_text(m_InsertLastSeenStatement, 1, Channel.c_str(), Channel.size(), SQLITE_STATIC);
	if(Result != SQLITE_OK)
	{
		Output::Error("SQLLite Plugin", { "Bind Channel for Insert LastSeen -> ", sqlite3_errmsg(m_dbHandle) });
		return;
	}

	Result = sqlite3_bind_text(m_InsertLastSeenStatement, 2, Nick.c_str(), Nick.size(), SQLITE_STATIC);
	if(Result != SQLITE_OK)
	{
		Output::Error("SQLLite Plugin", { "Bind Nick for Insert LastSeen-> ", sqlite3_errmsg(m_dbHandle) });
		return;
	}

	Result = sqlite3_bind_text(m_InsertLastSeenStatement, 3, Reason.c_str(), Reason.size(), SQLITE_STATIC);
	if(Result != SQLITE_OK)
	{
		Output::Error("SQLLite Plugin", { "Bind Reason for Insert LastSeen-> ", sqlite3_errmsg(m_dbHandle) });
		return;
	}

	while((Result = sqlite3_step(m_InsertLastSeenStatement))  != SQLITE_DONE)
	{
		if(Result != SQLITE_BUSY)
		{
			Output::Error("SQLLite Plugin", { "Executing the Insert LastSeen -> ", sqlite3_errmsg(m_dbHandle) });
			return;
		}		
	}
}

void CDatabaseWrapper::LastSeen(const std::string & Channel, const std::string & Nick, std::string & Time, std::string & Reason)
{	
	if(!Connected())
	{
		return;
	}

	if((Channel.size() <= 0) || (Nick.size() <= 0))
	{
		return;
	}

	int Result = 0;
	sqlite3_reset(m_GetLastSeenStatement);
	sqlite3_clear_bindings(m_GetLastSeenStatement);

	Result = sqlite3_bind_text(m_GetLastSeenStatement, 1, Channel.c_str(), Channel.size(), SQLITE_STATIC);
	if(Result != SQLITE_OK)
	{
		Output::Error("SQLLite Plugin", { "Bind Channel for Get LastSeen -> ", sqlite3_errmsg(m_dbHandle) });
		return;
	}

	Result = sqlite3_bind_text(m_GetLastSeenStatement, 2, Nick.c_str(), Nick.size(), SQLITE_STATIC);
	if(Result != SQLITE_OK)
	{
		Output::Error("SQLLite Plugin", { "Bind Nick for Get LastSeen-> ", sqlite3_errmsg(m_dbHandle) });
		return;
	}

	while((Result = sqlite3_step(m_GetLastSeenStatement)) == SQLITE_BUSY);

	if(Result == SQLITE_ROW)
	{
		Time = std::string(reinterpret_cast<const char*>(sqlite3_column_text(m_GetLastSeenStatement, 0)));
		Reason = std::string(reinterpret_cast<const char*>(sqlite3_column_text(m_GetLastSeenStatement, 1)));
	}

	if((Result != SQLITE_ROW) && (Result != SQLITE_DONE))
	{
		Output::Error("SQLLite Plugin", { "Executing the Get LastSeen -> ", sqlite3_errmsg(m_dbHandle) });
	}		
}



