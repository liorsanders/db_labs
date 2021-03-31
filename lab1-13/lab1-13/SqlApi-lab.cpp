#include <Windows.h>
#include <string>
#include <iostream>
#include <exception>
#include <sstream>
#include "sqlite3.h"
#include <io.h>

using std::string;
using std::stringstream;

void open_db(const char* name, sqlite3** db) {
	int res = sqlite3_open(name, db);
	if (res != SQLITE_OK) {
		*db = nullptr;
		throw std::runtime_error("failed to open DB");
	}
}

void my_exec(sqlite3* db, const char* sqlStatement) {
	std::cout << "executing: " << sqlStatement << std::endl;
	char* errMessage = nullptr;
	int res = sqlite3_exec(db, sqlStatement, nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK) {
		throw std::runtime_error(errMessage);
	}
}

void init_db(sqlite3** db) {
	//create the person table
	stringstream statement;
	statement << "CREATE TABLE IF NOT EXISTS PERSONS (" <<
		"ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," <<
		"LAST_NAME TEXT NOT NULL," <<
		"FIRST_NAME TEXT NOT NULL," <<
		"EMAIL TEXT NOT NULL," <<
		"PhonePrefix TEXT NOT NULL," <<
		"PhoneNumber TEXT NOT NULL);";
	my_exec(*db, statement.str().c_str());
	statement.str("CREATE TABLE IF NOT EXISTS PhonePrefixes (PhonePrefixID INTEGER PRIMARY KEY NOT NULL,Prefix TEXT NOT NULL);");
	//create the phone prefix table
	//statement << "CREATE TABLE IF NOT EXISTS PhonePrefixes (" <<
	//	"PhonePrefixID INTEGER PRIMARY KEY NOT NULL," <<
	//	"Prefix INTEGER NOT NULL);";
	my_exec(*db, statement.str().c_str());
	//create phone table
	statement.str("CREATE TABLE IF NOT EXISTS Phones (PhoneId INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,PhonePrefixID INTEGER null, FOREIGN KEY(PhonePrefixID) REFERENCES PhonePrefixes(PhonePrefixID));");
	my_exec(*db, statement.str().c_str());
}

int main() {
	sqlite3* db;
	string dbFileName = "PhoneBook.sqlite";

	int doesFileExist = _access(dbFileName.c_str(), 0);
	open_db(dbFileName.c_str(), &db);

	if (doesFileExist != 0) {
		try {
			init_db(&db);
		}
		catch (std::runtime_error& e) {
			std::cout << e.what() << std::endl;
		}
	}
	sqlite3_close(db);
	db = nullptr;
	
	return 0;
}
