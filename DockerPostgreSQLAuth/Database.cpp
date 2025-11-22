#include "Database.h"
#include <iostream>

std::unique_ptr<pqxx::connection> Database::conn = nullptr;

Database::Database() {
    if (!conn) {
        try {
            conn = std::make_unique<pqxx::connection>("postgresql://postgres:123@localhost:5432/AuthTest");
            if (conn->is_open()) {
                std::cout << "Opened database successfully: " << conn->dbname() << std::endl;
            } else {
                std::cout << "Can't open database" << std::endl;
                exit(1);
            }
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            exit(1);
        }
    }
}

pqxx::connection& Database::getInstance() {
    if (!conn) {
        static Database instance;
    }
    return *conn;
}
