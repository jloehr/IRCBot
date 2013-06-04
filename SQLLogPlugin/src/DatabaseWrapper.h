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
	void GetLog(const std::string & Channel, StringPairVector & OutVector);

	void UserLeft(const std::string & Channel, const std::string & Nick, const std::string & Reason);
	std::string LastSeen(const std::string & Nick);

private:
	const static std::string LogTableName;
	const static std::string LastSeenTableName;
	const static std::string PrepareQuery;
	const static std::string InsertLogQuery;
	const static std::string GetLogQuery;
	const static std::string InsertLastSeenQuery;
	const static std::string GetLastSeenQuery;

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