#include <Windows.h>
#include <string>
#include <iostream>
#include <exception>
#include <sstream>
#include "sqlite3.h"
#include <vector>
#include <io.h>

using std::string;
using std::stringstream;

void my_exec(sqlite3* db, const char* sqlStatement);

void update_info(sqlite3* db) {
	//1
	my_exec(db, "INSERT INTO PhonePrefixes (Prefix) VALUES ('089');");
	//2
	my_exec(db, "UPDATE PhonePrefixes SET Prefix = '078' WHERE Prefix = '089';");
	//3
	my_exec(db, "UPDATE PERSONS SET first_name = 'Sonia', last_name = 'Elimelech' WHERE ID = 1");
}

string enter_phones_helper(const string& phoneNumber, int n) {
	return "('" + std::to_string(n) + "', '" + phoneNumber + "', '" + std::to_string(n) + "'),";
}

void enter_phones(sqlite3* db) {
	std::vector<std::vector<string>> phones = {
		{"0222222222", "0245634567", "0285734561"},
		{"0333333333", "0345666643"},
		{"0444444444"}
	};
	string statement = "INSERT INTO PHONES (PhonePrefixID, PhoneNumber, PersonID) VALUES ";
	for (int i = 0; i < phones.size(); i++) {
		for (int j = 0; j < phones[i].size(); j++) {
			statement += enter_phones_helper(phones[i][j], i+1);
		}
	}
	statement[statement.size() - 1] = ';';
	my_exec(db, statement.c_str());
}

void enter_prefixes(sqlite3* db) {
	string statement = "INSERT INTO PhonePrefixes (Prefix) VALUES ";
	std::vector<string> prefixes = { "02", "03", "04", "08", "09", "050", "052", "053", "054", "055", "073", "077" };
	for (const string& p : prefixes) {
		statement = statement + "('" + p + "'),";
	}
	statement[statement.size() - 1] = ';';
	my_exec(db, statement.c_str());
}

void enter_people(sqlite3* db) {
	// lastName, //firstName, Email, PhonePrefix, PhoneNumber
	string statement = "INSERT INTO PERSONS (last_name, first_name, email, phonePrefix, phoneNumber) VALUES ";
	string seperator = "', '";
	std::vector<std::vector<string>> people = {
		{"tribiani", "joey", "joey@mango.com", "02", "0222222222"},
		{"geller", "ross", "ross@mango.com", "03", "0333333333"},
		{"bing", "chandler", "chandler@mango.com", "04", "0444444444"}
	};
	for (const auto& person : people) {
		statement = statement + "('" + person[0] + seperator + person[1] + seperator +
			person[2] + seperator + person[3] + seperator + person[4] + "'),";
	}
	statement[statement.size() - 1] = ';';
	my_exec(db, statement.c_str());
}

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
	statement.str("CREATE TABLE IF NOT EXISTS Phones (PhoneId INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,PhonePrefixID INTEGER null, PhoneNumber TEXT NOT NULL, PersonID INTEGER null ,FOREIGN KEY(PhonePrefixID) REFERENCES PhonePrefixes(PhonePrefixID), FOREIGN KEY(PersonID) REFERENCES PERSONS(PersonID));");
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
	//part 1
	enter_prefixes(db); //1
	enter_people(db); //2
	enter_phones(db); //3
	
	update_info(db); //4

	sqlite3_close(db);
	db = nullptr;

	return 0;
}
