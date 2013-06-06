/*
 *
 * Copyright (C) 2013 Julian Löhr
 * All rights reserved.
 *
 * This file is part of SQLLogPlugin, a Logging Plugin for TinyBot.
 * SQLLogPlugin is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
 * To read the full license, please check the LICENSE file.
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