#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include "MySQLDatabase.h"
#include <iostream>

MySQLDatabase::MySQLDatabase(const char* host, const char* user,const char* pass,const char* db, unsigned int port) {
	this->connection = mysql_init(NULL);

	if (!mysql_real_connect(connection, host, user, pass, db, port, NULL, 0)) {
		std::cout << "Erro ao conectar: " << getLastError() << std::endl;
	}
	else
	{
		std::cout << "Conexão sucedida" << std::endl;
	}
}

MySQLDatabase::~MySQLDatabase() {
	if(connection) mysql_close(connection);
}

std::string MySQLDatabase::getLastError(){
	this->lastError = mysql_error(connection);
	return this->lastError;
}

std::string MySQLDatabase::escape(std::string text) {
	char* text_safe = new char[text.length() * 2 + 1];
	mysql_real_escape_string(connection, text_safe, text.c_str(), text.length());
	std::string result(text_safe);
	delete[] text_safe;
	return result;
}

bool MySQLDatabase::execute(std::string sql){
	if (mysql_query(connection, sql.c_str())) {
		wxMessageBox("Erro ao executar SQL: " + getLastError(), "Erro", wxICON_ERROR);
		return false;
	}
	MYSQL_RES* res = mysql_store_result(connection);
	mysql_free_result(res);
	return true;
}

ull MySQLDatabase::getLastId() {
	return mysql_insert_id(connection);
}

int MySQLDatabase::getAffectedRows() {
	return mysql_affected_rows(connection);
}

MYSQL* MySQLDatabase::getSQL() {
	return connection;
}

std::vector<std::vector<std::string>> MySQLDatabase::query(std::string sql) {
	std::vector<std::vector<std::string>> matriz;

	if (mysql_query(connection, sql.c_str())) {
		return matriz;
	}

	MYSQL_RES* result = mysql_store_result(connection);
	if (!result) {
		return matriz;
	}

	int num_campos = mysql_num_fields(result);
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(result)))
	{
		std::vector<std::string> linha_vetor;
		linha_vetor.reserve(num_campos);
		for (int i = 0; i < num_campos; i++)
		{
			if (row[i])
				linha_vetor.emplace_back(row[i]);
			else
				linha_vetor.emplace_back();
		}
		matriz.push_back(std::move(linha_vetor));
	}

	mysql_free_result(result);
	return matriz;
}