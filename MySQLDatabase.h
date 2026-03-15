#pragma once
#include <iostream>
#include <vector>
#include <cstdint>
#include <mysql/mysql.h>
#include <wx/wx.h>

typedef UINT64 ull;

class MySQLDatabase
{
public:
	MySQLDatabase(const char* host, const char* user, const char* pass, const char* db, unsigned int port);
	~MySQLDatabase();

	bool execute(std::string sql);
	ull getLastId();
	int getAffectedRows();
	std::vector<std::vector<std::string>> query(std::string sql);
	std::string escape(std::string text);
	std::string getLastError(); 
	MYSQL* getSQL();

private:
	MYSQL* connection;
	std::string lastError;
};

