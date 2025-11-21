#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <cctype>

class Database {
    pqxx::connection conn;

    bool isValid(const std::string& str) {
        for (char c : str) {
            if (!std::isalnum(c)) {
                return false;
            }
        }
        return true;
    }

public:
    Database(const std::string& conn_str) : conn(conn_str) {
        if (conn.is_open()) {
            std::cout << "Opened database successfully: " << conn.dbname() << std::endl;
        }
        else {
            std::cout << "Can't open database" << std::endl;
            exit(1);
        }
    }

    void initialize() {
        try {
            pqxx::work w(conn);
            w.exec("CREATE TABLE IF NOT EXISTS users (id SERIAL PRIMARY KEY, username TEXT NOT NULL, password TEXT NOT NULL);");
            w.commit();
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    bool userExists(const std::string& name) {
        try {
            pqxx::work w(conn);
            pqxx::result r = w.exec("SELECT * FROM users WHERE username = " + w.quote(name));
            return !r.empty();
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            return false;
        }
    }

    bool registerUser(const std::string& name, const std::string& pass) {
        if (!isValid(name)) {
            std::cout << "Username contains special characters!" << std::endl;
            return false;
        }
        if (userExists(name)) {
            return false; // User already exists
        }
        try {
            pqxx::work w(conn);
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
            pqxx::work w(conn);
            pqxx::result r = w.exec("SELECT * FROM users WHERE username = " + w.quote(name) + " AND password = " + w.quote(pass));
            return !r.empty();
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            return false;
        }
    }

    void createUserTable(const std::string& username) {
        try {
            pqxx::work w(conn);
            w.exec("CREATE TABLE IF NOT EXISTS " + conn.quote_name(username) + " (id SERIAL PRIMARY KEY, data TEXT NOT NULL);");
            w.commit();
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void addElementToUserTable(const std::string& username, const std::string& data) {
        try {
            pqxx::work w(conn);
            w.exec("INSERT INTO " + conn.quote_name(username) + " (data) VALUES (" + w.quote(data) + ")");
            w.commit();
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void deleteUser(const std::string& username) {
        try {
            pqxx::work w(conn);
            w.exec("DELETE FROM users WHERE username = " + w.quote(username));
            w.exec("DROP TABLE IF EXISTS " + conn.quote_name(username));
            w.commit();
            std::cout << "User " << username << " and their table have been deleted." << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void deleteTable(const std::string& tableName) {
        try {
            pqxx::work w(conn);
            w.exec("DROP TABLE IF EXISTS " + conn.quote_name(tableName));
            w.commit();
            std::cout << "Table " << tableName << " has been deleted." << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    Database db("postgresql://postgres:123@localhost:5432/AuthTest");
    db.initialize();

    int authChoice;
    while (true) {
        system("cls");
        std::cout << std::string(10, '=') << "Authorization" << std::string(10, '=')
            << "\n1. Login\n"
            << "2. Register\n"
            << "3. Delete User\n"
            << "4. Delete Table\n"
            << "5. Exit Program\n"
            << "Your choice: ";
        std::cin >> authChoice;

        if (authChoice == 1) {
            system("cls");
            std::string l, p;
            std::cout << "Login: ";
            std::cin >> l;
            std::cout << "Password: ";
            std::cin >> p;
            if (db.loginUser(l, p)) {
                db.createUserTable(l);
                while (true) {
                    system("cls");
                    std::cout << "You are logged in as " << l << std::endl;
                    std::cout << "1. Add element" << std::endl;
                    std::cout << "2. Logout" << std::endl;
                    int choice;
                    std::cin >> choice;
                    if (choice == 1) {
                        std::string data;
                        std::cout << "Enter data: ";
                        std::cin >> data;
                        db.addElementToUserTable(l, data);
                    }
                    else if (choice == 2) {
                        break;
                    }
                }
            }
            else {
                std::cout << "\n\nwrong password or login";
                system("pause");
            }
        }
        else if (authChoice == 2) {
            system("cls");
            std::string l, p;
            std::cout << "Create user\nLogin: ";
            std::cin >> l;
            std::cout << "Password: ";
            std::cin >> p;

            if (db.registerUser(l, p)) {
                std::cout << "User registered successfully!" << std::endl;
            }
            else {
                std::cout << "User already exists or username is invalid!" << std::endl;
            }
            system("pause");
        }
        else if (authChoice == 3) {
            system("cls");
            std::string l;
            std::cout << "Enter username to delete: ";
            std::cin >> l;
            if (db.userExists(l)) {
                db.deleteUser(l);
            }
            else {
                std::cout << "User does not exist!" << std::endl;
            }
            system("pause");
        }
        else if (authChoice == 4) {
            system("cls");
            std::string tableName;
            std::cout << "Enter table name to delete: ";
            std::cin >> tableName;
            db.deleteTable(tableName);
            system("pause");
        }
        else if (authChoice == 5) return 0;
    }
}