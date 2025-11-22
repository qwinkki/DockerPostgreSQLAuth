#pragma once
#include <pqxx/pqxx>

class Database {
public:
    static pqxx::connection& getInstance();

private:
    Database();
    static std::unique_ptr<pqxx::connection> conn;
};
