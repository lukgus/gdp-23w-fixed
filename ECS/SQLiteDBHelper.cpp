#include "SQLiteDBHelper.h"

#include <iostream>

int ResultCallback(void*, int columns, char** text, char** names)
{
	printf("Result: \n");
	printf(" num columns: %d\n", columns);

	for (int i = 0; i < columns; i++)
	{
		printf("%s\n", text[i]);
	}
	for (int i = 0; i < columns; i++)
	{
		printf("%s\n", names[i]);
	}

	return 0;
}

SQLiteDBHelper::SQLiteDBHelper()
	: m_DB(nullptr)
	, m_IsConnected(false)
{
}

SQLiteDBHelper::~SQLiteDBHelper()
{
	if (m_DB)
	{
		printf("SQLiteDBHelper::~SQLiteDBHelper: Closed connection to our DB\n");
		sqlite3_close(m_DB);
	}
}

void SQLiteDBHelper::ConnectToDB(const char* dbName)
{
	if (m_IsConnected)
	{
		printf("SQLiteDBHelper::ConnectToDB: Already connected to a DB\n");
		return;
	}

	int result = sqlite3_open(dbName, &m_DB);
	if (result != SQLITE_OK)
	{
		printf("Failed to open our SQLite DB '%s' with error code: %d\n", dbName, result);
		return;
	}

	m_IsConnected = true;
}

void SQLiteDBHelper::ExecuteQuery(const char* sql)
{
	if (!m_IsConnected)
	{
		printf("SQLiteDBHelper::ExecuteQuery: not connected to a DB\n");
		return;
	}

	char* errorMsg;
	//void* arg;
	int result = sqlite3_exec(m_DB, sql, ResultCallback, nullptr, &errorMsg);
	if (result != SQLITE_OK)
	{
		printf("Failed to execute our query with erro code: %d!\n", result);
		return;
	}
}