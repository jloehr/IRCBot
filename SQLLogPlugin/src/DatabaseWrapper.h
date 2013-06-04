#pragma once

#include <string>
#include <sqlite3.h>

#include "Output.h"

class CDatabaseWrapper
{
public:
	static CDatabaseWrapper Singleton;

	CDatabaseWrapper();
	~CDatabaseWrapper();

	bool Connected();

private:
	const static std::string LogTableName;
	const static std::string LastSeenTableName;
	const static std::string LogChannelParameter;
	const static std::string LogMessageParameter;
	const static std::string LastSeenChannelParameter;
	const static std::string LastSeenNickParameter;
	const static std::string PrepareQuery;
	const static std::string InsertLogQuery;
	const static std::string GetLogQuery;
	const static std::string InsertLastSeenQuery;
	const static std::string GetLastSeenQuery;

	sqlite3 * m_dbHandle;

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