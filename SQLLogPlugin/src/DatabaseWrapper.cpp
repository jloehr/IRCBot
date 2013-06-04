#include "DatabaseWrapper.h"

const std::string CDatabaseWrapper::LogTableName = std::string("chatlog");
const std::string CDatabaseWrapper::LastSeenTableName = std::string("lastseen");
const std::string CDatabaseWrapper::LogChannelParameter = std::string("Channel");
const std::string CDatabaseWrapper::LogMessageParameter = std::string("Message");
const std::string CDatabaseWrapper::LastSeenChannelParameter = std::string("Channel");
const std::string CDatabaseWrapper::LastSeenNickParameter = std::string("Nick");
const std::string CDatabaseWrapper::PrepareQuery = std::string("CREATE TABLE IF NOT EXISTS "+CDatabaseWrapper::LogTableName+" (Time INTEGER, Channel TEXT, Message TEXT); CREATE TABLE IF NOT EXISTS "+CDatabaseWrapper::LastSeenTableName+" (Time INTEGER, Channel TEXT, Nick TEXT);");
const std::string CDatabaseWrapper::InsertLogQuery = std::string("INSERT INTO "+CDatabaseWrapper::LogTableName+" VALUES (strftime('%s','now'), ':"+CDatabaseWrapper::LogChannelParameter+"', ':"+CDatabaseWrapper::LogMessageParameter+"');");
const std::string CDatabaseWrapper::GetLogQuery = std::string("SELCET strftime('%d.%m.%Y %M:%H', Time, 'unixepoch') AS TimeStamp, Message FROM "+CDatabaseWrapper::LogTableName+" WHERE Channel = ':"+CDatabaseWrapper::LogChannelParameter+"' ORDER BY Time ASC;");
const std::string CDatabaseWrapper::InsertLastSeenQuery = std::string("INSERT INTO "+CDatabaseWrapper::LastSeenTableName+" VALUES(strftime('%s','now'), ':"+CDatabaseWrapper::LastSeenChannelParameter+"', ':"+CDatabaseWrapper::LastSeenNickParameter+"');");
const std::string CDatabaseWrapper::GetLastSeenQuery = std::string("SELECT strftime('%d.%m.%Y %M:%H', Time, 'unixepoch') AS TimeStamp FROM "+CDatabaseWrapper::LastSeenTableName+" WHERE (Channel = ':"+CDatabaseWrapper::LastSeenChannelParameter+"') AND (Nick = ':"+CDatabaseWrapper::LastSeenNickParameter+"') ORDER BY Time ASC;");

CDatabaseWrapper CDatabaseWrapper::Singleton = CDatabaseWrapper();

//------------------------------------------//
//											//
//		  Konstructor & Destructor			//
//											//
//------------------------------------------//

CDatabaseWrapper::CDatabaseWrapper()
	:m_dbHandle(NULL)
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

	/*
	sqlite3_stmt * PrepareStatement;
	int Result = sqlite3_prepare_v2(m_dbHandle, CDatabaseWrapper::PrepareQuery.c_str(), CDatabaseWrapper::PrepareQuery.size(), &PrepareStatement, NULL);

	Output::Log("SQLLite Plugin", { "Statement Content -> ", sqlite3_sql(PrepareStatement)});

	if(Result != SQLITE_OK)
	{
		Output::Error("SQLLite Plugin", { "On preparing the initial statement -> ", sqlite3_errmsg(m_dbHandle) });
		sqlite3_finalize(PrepareStatement);
		CloseDB();
		return;
	}

	while((Result = sqlite3_step(PrepareStatement)) != SQLITE_DONE)
	{
		if(Result != SQLITE_BUSY)
		{
			Output::Error("SQLLite Plugin", { "On executin the initial statement -> ", sqlite3_errmsg(m_dbHandle) });
			sqlite3_finalize(PrepareStatement);
			CloseDB();
			return;
		}
		else
		{
			Output::Log("SQLLite Plugin", { "sqlite step is Busy"});
		}
	}

	sqlite3_finalize(PrepareStatement);
	*/
}

void CDatabaseWrapper::PrepareStatements()
{

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

}