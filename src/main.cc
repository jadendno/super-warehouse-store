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
                      // Members
    const char* sql = "CREATE TABLE IF NOT EXISTS Members("
                      "member_number INTEGER PRIMARY KEY, "
                      "name TEXT NOT NULL, "
                      "membership_type TEXT NOT NULL CHECK (membership_type IN ('basic', 'preferred')), "
                      "expiration_date DATE NOT NULL, "
                      "total_spent REAL DEFAULT 0.0, "
                      "rebate_amount REAL DEFAULT 0.0); "
                      // Items
                      "CREATE TABLE IF NOT EXISTS Items ("
                      "item_id INTEGER PRIMARY KEY, "
                      "item_name TEXT NOT NULL UNIQUE); "
                      // Transactions
                      "CREATE TABLE IF NOT EXISTS Transactions ("
                      "transaction_id INTEGER PRIMARY KEY, "
                      "member_number INTEGER NOT NULL, "
                      "transaction_date DATE NOT NULL, "
                      "total_amount REAL, "
                      "FOREIGN KEY (member_number) REFERENCES Members(member_number)); "
                      // TransactionItems
                      "CREATE TABLE IF NOT EXISTS TransactionItems ("
                      "transaction_item_id INTEGER PRIMARY KEY, "
                      "transaction_id INTEGER NOT NULL, "
                      "item_id INTEGER NOT NULL, "
                      "quantity INTEGER NOT NULL, "
                      "sale_price REAL NOT NULL, "
                      "FOREIGN KEY (transaction_id) REFERENCES Transactions(transaction_id)"
                      "FOREIGN KEY (item_id) REFERENCES Items(item_id));";

    exit = sqlite3_exec(db, sql, 0, 0, nullptr);
    if (exit != SQLITE_OK) {
        std::cerr << "Error creating table\n";
    } else {
        std::cout << "Table created successfully\n";
    }

    /*const char* insertSQL = "INSERT INTO Users (Name, Age) VALUES ('Alice', 25);";
    exit = sqlite3_exec(db, insertSQL, 0, 0, nullptr);
    if (exit != SQLITE_OK) {
        std::cerr << "Error inserting data\n";
    } else {
        std::cout << "Data inserted successfully\n";
    }


    const char* selectSQL = "SELECT * FROM Users;";
    exit = sqlite3_exec(db, selectSQL, callback, nullptr, nullptr);
    if (exit != SQLITE_OK) {
        std::cerr << "Error selecting data\n";
    }

    sqlite3_close(db);*/
    return 0;
}
