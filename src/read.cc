#include <iostream>
#include <sqlite3.h>

static int callback(void* data, int argc, char** argv, char** colNames) {
    for (int i = 0; i < argc; i++) {
        std::cout << colNames[i] << ": " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    std::cout << "-----------------\n";
    return 0;
}

int main(void) {
    sqlite3* db;
    int exit = sqlite3_open("warehouse.db", &db);
    if (exit) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Database opened successfully!" << std::endl;
    }

    const char* selectSQL = "SELECT * FROM Users;";
    exit = sqlite3_exec(db, selectSQL, callback, nullptr, nullptr);
    if (exit != SQLITE_OK) {
        std::cerr << "Error selecting data\n";
    }

    sqlite3_close(db);
    return 0;
}
