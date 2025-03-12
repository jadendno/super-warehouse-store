#include <database.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <iomanip>
#include <sqlite3.h>

sqlite3* OpenDb(const char* name) {
    sqlite3* db;
    int exit = sqlite3_open(name, &db);

    if (!exit) {
        std::cout << "Database opened successfully!" << std::endl;
        return db;
    }
    
    const std::string error = "Error opening database: " + std::string(sqlite3_errmsg(db)) + "\n";
    throw std::runtime_error(error);
}

namespace init {
    namespace {
        // Convert a string to lowercase
        std::string to_lower(const std::string &s) {
            std::string lower = s;
            std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
            return lower;
        }

        // Convert date from MM/DD/YYYY to YYYY-MM-DD using std::get_time and std::put_time
        std::string convert_date(const std::string &date_str) {
            std::tm tm = {};
            std::istringstream ss(date_str);
            ss >> std::get_time(&tm, "%m/%d/%Y");
            if (ss.fail()) {
                throw std::runtime_error("Failed to parse date: " + date_str);
            }
            std::ostringstream oss;
            oss << std::put_time(&tm, "%Y-%m-%d");
            return oss.str();
        }
        
        // Helper: Trim whitespace from both ends of a string.
        std::string trim(const std::string &s) {
            size_t start = s.find_first_not_of(" \t\n\r");
            size_t end = s.find_last_not_of(" \t\n\r");
            return (start == std::string::npos || end == std::string::npos) ? "" : s.substr(start, end - start + 1);
        }
        
        void InitializeTables(const char* name) {
            sqlite3* db = OpenDb(name);

            const char* kMembersSql =          "CREATE TABLE IF NOT EXISTS Members ("
                                               "member_number INTEGER PRIMARY KEY, "
                                               "name TEXT NOT NULL, "
                                               "membership_type TEXT NOT NULL CHECK (membership_type IN ('basic', 'preferred')), "
                                               "expiration_date DATE NOT NULL, "
                                               "total_spent REAL DEFAULT 0.0, "
                                               "rebate_amount REAL DEFAULT 0.0);";
                         
            const char* kItemsSql =            "CREATE TABLE IF NOT EXISTS Items ("
                                               "item_id INTEGER PRIMARY KEY, "
                                               "item_name TEXT NOT NULL UNIQUE);";
                                 
            const char* kTransactionsSql =     "CREATE TABLE IF NOT EXISTS Transactions (" 
                                               "transaction_id INTEGER PRIMARY KEY, "
                                               "member_number INTEGER NOT NULL, "
                                               "transaction_date DATE NOT NULL, "
                                               "total_amount REAL, "
                                               "FOREIGN KEY (member_number) REFERENCES Members(member_number));";

            const char* kTransactionItemsSql = "CREATE TABLE IF NOT EXISTS TransactionItems ("
                                               "transaction_item_id INTEGER PRIMARY KEY, "
                                               "transaction_id INTEGER NOT NULL, "
                                               "item_id INTEGER NOT NULL, "
                                               "quantity INTEGER NOT NULL, "
                                               "sale_price REAL NOT NULL, "
                                               "FOREIGN KEY (transaction_id) REFERENCES Transactions(transaction_id)"
                                               "FOREIGN KEY (item_id) REFERENCES Items(item_id));";

            int exit = sqlite3_exec(db, kMembersSql, 0, 0, nullptr);
            if (exit != SQLITE_OK) {
                throw std::runtime_error("Error creating table Members\n");
            } else {
                std::cout << "Table Members created successfully\n";
            }

            exit = sqlite3_exec(db, kItemsSql, 0, 0, nullptr);
            if (exit != SQLITE_OK) {
                throw std::runtime_error("Error creating table Items\n");
            } else {
                std::cout << "Table Items created successfully\n";
            }

            exit = sqlite3_exec(db, kTransactionsSql, 0, 0, nullptr);
            if (exit != SQLITE_OK) {
                throw std::runtime_error("Error creating table Transactions\n");
            } else {
                std::cout << "Table Transactions created successfully\n";
            }

            exit = sqlite3_exec(db, kTransactionItemsSql, 0, 0, nullptr);
            if (exit != SQLITE_OK) {
                throw std::runtime_error("Error creating table TransactionItems\n");
            } else {
                std::cout << "Table TransactionItems created successfully\n";
            }
    
            sqlite3_close(db);
        }
        
        void AddMembersFromTxt(const char* name, const std::string &filename) {
            sqlite3* db = OpenDb(name);
            
            std::ifstream infile(filename);
            if (!infile.is_open()) {
                std::cerr << "Cannot open file: " << filename << "\n";
                return;
            }
    
            // Read and trim non-empty lines
            std::vector<std::string> lines;
            std::string line;
            while (std::getline(infile, line)) {
                // Trim leading and trailing whitespace
                line.erase(0, line.find_first_not_of(" \t\n\r"));
                line.erase(line.find_last_not_of(" \t\n\r") + 1);
                if (!line.empty()) {
                    lines.push_back(line);
                }
            }
            infile.close();
    
            // Validate file format
            if (lines.size() % 4 != 0) {
                std::cerr << "File format error: The number of non-empty lines is not a multiple of 4.\n";
                return;
            }
    
            // Process each record (4 lines per member)
            for (size_t i = 0; i < lines.size(); i += 4) {
                std::string name = lines[i];
                std::string member_id_line = lines[i + 1];
                std::string membership_type = to_lower(lines[i + 2]);
                std::string expiration_raw = lines[i + 3];
        
                int member_number;
                try {
                    member_number = std::stoi(member_id_line);
                } catch (const std::exception &e) {
                    std::cerr << "Error parsing member ID on line " << i + 2 << ": " << e.what() << "\n";
                    continue;
                }
        
                std::string expiration_date;
                try {
                    expiration_date = convert_date(expiration_raw);
                } catch (const std::exception &e) {
                    std::cerr << "Error parsing expiration date on line " << i + 4 << ": " << e.what() << "\n";
                    continue;
                }
        
                // Check if the member already exists in the database
                const char* select_sql = "SELECT member_number FROM Members WHERE member_number = ?";
                sqlite3_stmt *stmt;
                if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr) != SQLITE_OK) {
                    std::cerr << "Failed to prepare SELECT statement: " << sqlite3_errmsg(db) << "\n";
                    continue;
                }
                sqlite3_bind_int(stmt, 1, member_number);
                int rc = sqlite3_step(stmt);
                bool exists = (rc == SQLITE_ROW);
                sqlite3_finalize(stmt);
        
                if (!exists) {
                    // Insert new member
                    const char* insert_sql = "INSERT INTO Members (member_number, name, membership_type, expiration_date, total_spent, rebate_amount) VALUES (?, ?, ?, ?, 0.0, 0.0)";
                    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) != SQLITE_OK) {
                        std::cerr << "Failed to prepare INSERT statement: " << sqlite3_errmsg(db) << "\n";
                        continue;
                    }
                    sqlite3_bind_int(stmt, 1, member_number);
                    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 3, membership_type.c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 4, expiration_date.c_str(), -1, SQLITE_TRANSIENT);
            
                    if (sqlite3_step(stmt) != SQLITE_DONE) {
                        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << "\n";
                    } else {
                        std::cout << "Inserted new member: " << name << " (ID: " << member_number << ")\n";
                    }
                    sqlite3_finalize(stmt);
                } else {
                    // Update existing member
                    const char* update_sql = "UPDATE Members SET name = ?, membership_type = ?, expiration_date = ? WHERE member_number = ?";
                    if (sqlite3_prepare_v2(db, update_sql, -1, &stmt, nullptr) != SQLITE_OK) {
                        std::cerr << "Failed to prepare UPDATE statement: " << sqlite3_errmsg(db) << "\n";
                        continue;
                    }
                    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 2, membership_type.c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 3, expiration_date.c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_int(stmt, 4, member_number);
            
                    if (sqlite3_step(stmt) != SQLITE_DONE) {
                        std::cerr << "Update failed: " << sqlite3_errmsg(db) << "\n";
                    } else {
                        std::cout << "Updated member: " << name << " (ID: " << member_number << ")\n";
                    }
                    sqlite3_finalize(stmt);
                }
            }
        }

        void AddTransactionsFromTxt(const char* name, const std::string filename) {
            sqlite3* db = OpenDb(name);
            
            std::ifstream infile(filename);
            if (!infile) {
                std::cerr << "Cannot open file: " << filename << "\n";
                return;
            }

            std::vector<std::string> lines;
            std::string line;
            while (std::getline(infile, line)) {
                line = trim(line);
                if (!line.empty()) {
                    lines.push_back(line);
                }
            }
            infile.close();

            if (lines.size() % 4 != 0) {
                std::cerr << "File format error: The number of non-empty lines is not a multiple of 4.\n";
                return;
            }

            for (size_t i = 0; i < lines.size(); i += 4) {
                std::string date_line = lines[i];
                std::string member_id_line = lines[i+1];
                std::string item_desc = lines[i+2];
                std::string price_qty_line = lines[i+3];

                double sale_price = 0.0;
                int quantity = 0;
                {
                    std::istringstream iss(price_qty_line);
                    if (!(iss >> sale_price >> quantity)) {
                        std::cerr << "Error parsing price and quantity on block starting at line " << i+1 << "\n";
                        continue;
                    }
                }
    
                int member_id = 0;
                try {
                    member_id = std::stoi(member_id_line);
                } catch (const std::exception &e) {
                    std::cerr << "Error parsing member ID on line " << i+2 << ": " << e.what() << "\n";
                    continue;
                }
    
                double transaction_total = sale_price * quantity;
                sqlite3_stmt *stmt = nullptr;
                int rc;

                // Check if member exists
                const char* select_member = "SELECT member_number FROM Members WHERE member_number = ?";
                rc = sqlite3_prepare_v2(db, select_member, -1, &stmt, nullptr);
                if (rc != SQLITE_OK) {
                    std::cerr << "Failed to prepare SELECT statement: " << sqlite3_errmsg(db) << "\n";
                    continue;
                }
                sqlite3_bind_int(stmt, 1, member_id);
                bool member_exists = (sqlite3_step(stmt) == SQLITE_ROW);
                sqlite3_finalize(stmt);

                if (!member_exists) {
                    // Insert member with default values: a default name and a placeholder expiration date.
                    const char* insert_member = "INSERT INTO Members (member_number, name, membership_type, expiration_date, total_spent, rebate_amount) VALUES (?, ?, 'basic', '1970-01-01', 0.0, 0.0)";
                    rc = sqlite3_prepare_v2(db, insert_member, -1, &stmt, nullptr);
                    if (rc != SQLITE_OK) {
                        std::cerr << "Failed to prepare INSERT for member: " << sqlite3_errmsg(db) << "\n";
                        continue;
                    }
                    std::string default_name = "Member " + std::to_string(member_id);
                    sqlite3_bind_int(stmt, 1, member_id);
                    sqlite3_bind_text(stmt, 2, default_name.c_str(), -1, SQLITE_TRANSIENT);
                    if (sqlite3_step(stmt) != SQLITE_DONE) {
                        std::cerr << "Insert member failed: " << sqlite3_errmsg(db) << "\n";
                    }
                    sqlite3_finalize(stmt);
                }
    
                // Insert new transaction
                const char* insert_trans = "INSERT INTO Transactions (member_number, transaction_date, total_amount) VALUES (?, ?, ?)";
                rc = sqlite3_prepare_v2(db, insert_trans, -1, &stmt, nullptr);
                if (rc != SQLITE_OK) {
                    std::cerr << "Failed to prepare INSERT for transaction: " << sqlite3_errmsg(db) << "\n";
                    continue;
                }
                sqlite3_bind_int(stmt, 1, member_id);
                sqlite3_bind_text(stmt, 2, date_line.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_double(stmt, 3, transaction_total);
                if (sqlite3_step(stmt) != SQLITE_DONE) {
                    std::cerr << "Insert transaction failed: " << sqlite3_errmsg(db) << "\n";
                    sqlite3_finalize(stmt);
                    continue;
                }
                int transaction_id = (int)sqlite3_last_insert_rowid(db);
                sqlite3_finalize(stmt);
    
                // Get or insert the item in the Items table
                int item_id = 0;
                const char* select_item = "SELECT item_id FROM Items WHERE item_name = ?";
                rc = sqlite3_prepare_v2(db, select_item, -1, &stmt, nullptr);
                if (rc != SQLITE_OK) {
                    std::cerr << "Failed to prepare SELECT for item: " << sqlite3_errmsg(db) << "\n";
                    continue;
                }
                sqlite3_bind_text(stmt, 1, item_desc.c_str(), -1, SQLITE_TRANSIENT);
                if (sqlite3_step(stmt) == SQLITE_ROW) {
                    item_id = sqlite3_column_int(stmt, 0);
                }
                sqlite3_finalize(stmt);
    
                if (item_id == 0) {
                    const char* insert_item = "INSERT INTO Items (item_name) VALUES (?)";
                    rc = sqlite3_prepare_v2(db, insert_item, -1, &stmt, nullptr);
                    if (rc != SQLITE_OK) {
                        std::cerr << "Failed to prepare INSERT for item: " << sqlite3_errmsg(db) << "\n";
                        continue;
                    }
                    sqlite3_bind_text(stmt, 1, item_desc.c_str(), -1, SQLITE_TRANSIENT);
                    if (sqlite3_step(stmt) != SQLITE_DONE) {
                        std::cerr << "Insert item failed: " << sqlite3_errmsg(db) << "\n";
                    }
                    item_id = (int)sqlite3_last_insert_rowid(db);
                    sqlite3_finalize(stmt);
                }
    
                // Insert into TransactionItems table
                const char* insert_trans_item = "INSERT INTO TransactionItems (transaction_id, item_id, quantity, sale_price) VALUES (?, ?, ?, ?)";
                rc = sqlite3_prepare_v2(db, insert_trans_item, -1, &stmt, nullptr);
                if (rc != SQLITE_OK) {
                    std::cerr << "Failed to prepare INSERT for TransactionItems: " << sqlite3_errmsg(db) << "\n";
                    continue;
                }
                sqlite3_bind_int(stmt, 1, transaction_id);
                sqlite3_bind_int(stmt, 2, item_id);
                sqlite3_bind_int(stmt, 3, quantity);
                sqlite3_bind_double(stmt, 4, sale_price);
                if (sqlite3_step(stmt) != SQLITE_DONE) {
                    std::cerr << "Insert TransactionItem failed: " << sqlite3_errmsg(db) << "\n";
                }
                sqlite3_finalize(stmt);
    
                // Update member's total_spent
                const char* update_member = "UPDATE Members SET total_spent = total_spent + ? WHERE member_number = ?";
                rc = sqlite3_prepare_v2(db, update_member, -1, &stmt, nullptr);
                if (rc != SQLITE_OK) {
                    std::cerr << "Failed to prepare UPDATE for member: " << sqlite3_errmsg(db) << "\n";
                    continue;
                }
                sqlite3_bind_double(stmt, 1, transaction_total);
                sqlite3_bind_int(stmt, 2, member_id);
                if (sqlite3_step(stmt) != SQLITE_DONE) {
                    std::cerr << "Update member total_spent failed: " << sqlite3_errmsg(db) << "\n";
                }
                sqlite3_finalize(stmt);
    
                // Commit after processing each transaction (SQLite commits automatically unless in a transaction)
                std::cout << "Processed transaction for member " << member_id 
                          << " on " << date_line << ": " << quantity 
                          << " x '" << item_desc << "' at $" << sale_price 
                          << " each." << std::endl;
            }
        }
    }
    
    void Init(const char* name) {
        InitializeTables(name);
        AddMembersFromTxt(name, "data/warehouse-shoppers.txt");
        AddTransactionsFromTxt(name, "data/day1.txt");
        AddTransactionsFromTxt(name, "data/day2.txt");
        AddTransactionsFromTxt(name, "data/day3.txt");
        AddTransactionsFromTxt(name, "data/day4.txt");
        AddTransactionsFromTxt(name, "data/day5.txt");
    }
}
