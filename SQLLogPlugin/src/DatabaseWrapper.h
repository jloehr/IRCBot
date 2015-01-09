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
#pragma once

#include <string>
#include <utility>
#include <vector>
#include <sqlite3.h>

#include "Output.h"

typedef std::pair<std::string, std::string> StringPair;
typedef std::vector<StringPair> StringPairVector;

class CDatabaseWrapper
{
public:
	static CDatabaseWrapper Singleton;

	CDatabaseWrapper();
	~CDatabaseWrapper();

	bool Connected();

	void LogMessage(const std::string & Channel, const std::string & Message);
	void GetLog(const std::string & Channel, const int limit, const int offset, StringPairVector & OutVector);

	void UserLeft(const std::string & Channel, const std::string & Nick, const std::string & Reason);
	void LastSeen(const std::string & Channel, const std::string & Nick, std::string & Time, std::string & Reason);

private:
	static const std::string LogTableName;
	static const std::string LastSeenTableName;
	static const std::string PrepareQuery;
	static const std::string InsertLogQuery;
	static const std::string GetLogQuery;
	static const std::string InsertLastSeenQuery;
	static const std::string GetLastSeenQuery;

	sqlite3 * m_dbHandle;
	sqlite3_stmt * m_InsertLogStatement;
	sqlite3_stmt * m_GetLogStatement;
	sqlite3_stmt * m_InsertLastSeenStatement;
	sqlite3_stmt * m_GetLastSeenStatement;

	void OpenDB();
	void PrepareDatabase();
	void FinalizeStatements();
	void CloseDB();
	void PrepareStatements();
};

inline
bool CDatabaseWrapper::Connected()
{
	return (m_dbHandle != NULL);
}