#include <iostream>
#include <pqxx/pqxx>
#include <string>

class Database {
    pqxx::connection conn;

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

    bool registerUser(const std::string& name, const std::string& pass) {
        try {
            pqxx::work w(conn);
            pqxx::result r = w.exec("SELECT * FROM users WHERE username = " + w.quote(name));
            if (!r.empty()) {
                return false; // User already exists
            }
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
};

int main() {
    setlocale(LC_ALL, "Russian");
    Database db("postgresql://postgres:123@localhost:5432/AuthTest");
    db.initialize();

    int authChoice;
    while (true) {
        system("cls");
        std::cout << std::string(10, '=') << "Autorization" << std::string(10, '=')
            << "\n1. Login\n"
            << "2. Register\n"
            << "3. Exit Program\n"
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
                std::cout << "User already exists!" << std::endl;
            }
            system("pause");
        }
        else if (authChoice == 3) return 0;

    }
}