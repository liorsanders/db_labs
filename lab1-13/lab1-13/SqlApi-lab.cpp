#include <Windows.h>
#include <string>
#include <iostream>
#include <exception>
#include "sqlite3.h"
#include <io.h>

using namespace std;

void open_db(const char* name, sqlite3** db) {
	int res = sqlite3_open(name, db);
	if (res != SQLITE_OK) {
		*db = nullptr;
		throw std::runtime_error("failed to open DB");
	}
}

int main() {
	sqlite3* db;
	
	return 0;
}