#include "Auth.h"
#include "Database.h"
#include <iostream>
#include <pqxx/pqxx>
#include <cctype>

namespace {
    bool isValid(const std::string& str) {
        for (char c : str) {
            if (!std::isalnum(c)) {
                return false;
            }
        }
        return true;
    }

    bool userExists(const std::string& name) {
        try {
            pqxx::work w(Database::getInstance());
            pqxx::result r = w.exec("SELECT * FROM users WHERE username = " + w.quote(name));
            return !r.empty();
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            return false;
        }
    }
}

void initialize_database() {
    try {
        pqxx::work w(Database::getInstance());
        w.exec("CREATE TABLE IF NOT EXISTS users (id SERIAL PRIMARY KEY, username TEXT NOT NULL UNIQUE, password TEXT NOT NULL);");
        w.commit();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

bool registerUser(const std::string& name, const std::string& pass) {
    if (!isValid(name)) {
        std::cout << "Username contains special characters!" << std::endl;
        return false;
    }
    if (userExists(name)) {
        std::cout << "User already exists!" << std::endl;
        return false;
    }
    try {
        pqxx::work w(Database::getInstance());
        w.exec("INSERT INTO users (username, password) VALUES (" + w.quote(name) + ", " + w.quote(pass) + ")");
        w.commit();
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

bool loginUser(const std::string& name, const std::string& pass) {
    try {
        pqxx::work w(Database::getInstance());
        pqxx::result r = w.exec("SELECT * FROM users WHERE username = " + w.quote(name) + " AND password = " + w.quote(pass));
        return !r.empty();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

void viewAllUsers() {
    try {
        pqxx::work w(Database::getInstance());
        pqxx::result r = w.exec("SELECT id, username FROM users ORDER BY id;");
        
        system("cls");
        std::cout << "Registered Users:" << std::endl;
        std::cout << "ID\tUsername" << std::endl;
        std::cout << "-----------------" << std::endl;
        if (r.empty()) {
            std::cout << "No users found." << std::endl;
        } else {
            for (const auto& row : r) {
                std::cout << row["id"].as<int>() << "\t" << row["username"].as<std::string>() << std::endl;
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void deleteUserById() {
    viewAllUsers();
    int id;
    std::cout << "\nEnter the ID of the user to delete: ";
    std::cin >> id;

    if (std::cin.fail()) {
        std::cout << "Invalid input. Please enter a number." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    try {
        pqxx::work w(Database::getInstance());
        // Also get the username to drop the corresponding table
        pqxx::result r = w.exec("SELECT username FROM users WHERE id = " + w.quote(id));
        if (r.empty()) {
            std::cout << "User with ID " << id << " not found." << std::endl;
            return;
        }
        std::string username = r[0]["username"].as<std::string>();

        w.exec("DELETE FROM users WHERE id = " + w.quote(id));
        w.exec("DROP TABLE IF EXISTS " + w.conn().quote_name(username));
        w.commit();
        std::cout << "User with ID " << id << " (" << username << ") has been deleted." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}


void createUserTable(const std::string& username) {
    try {
        pqxx::work w(Database::getInstance());
        w.exec("CREATE TABLE IF NOT EXISTS " + w.conn().quote_name(username) + " (id SERIAL PRIMARY KEY, data TEXT NOT NULL);");
        w.commit();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void addElementToUserTable(const std::string& username, const std::string& data) {
    try {
        pqxx::work w(Database::getInstance());
        w.exec("INSERT INTO " + w.conn().quote_name(username) + " (data) VALUES (" + w.quote(data) + ")");
        w.commit();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
