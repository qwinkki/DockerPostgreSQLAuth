#include <iostream>
#include <string>
#include "Auth.h"

void loggedInMenu(const std::string& username) {
    while (true) {
        system("cls");
        std::cout << "You are logged in as " << username << std::endl;
        std::cout << "1. Add element" << std::endl;
        std::cout << "2. Logout" << std::endl;
        
        int choice;
        std::cout << "Your choice: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            system("pause");
            continue;
        }

        if (choice == 1) {
            std::string data;
            std::cout << "Enter data: ";
            std::cin.ignore(); // consume newline
            std::getline(std::cin, data);
            addElementToUserTable(username, data);
        }
        else if (choice == 2) {
            break;
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    initialize_database();

    while (true) {
        system("cls");
        std::cout << std::string(10, '=') << " Main Menu " << std::string(10, '=') << std::endl;
        std::cout << "1. Login" << std::endl;
        std::cout << "2. Register" << std::endl;
        std::cout << "3. View All Users" << std::endl;
        std::cout << "4. Delete User by ID" << std::endl;
        std::cout << "5. Exit Program" << std::endl;
        std::cout << "Your choice: ";

        int authChoice;
        std::cin >> authChoice;

        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            system("pause");
            continue;
        }

        if (authChoice == 1) {
            system("cls");
            std::string l, p;
            std::cout << "Login: ";
            std::cin >> l;
            std::cout << "Password: ";
            std::cin >> p;
            if (loginUser(l, p)) {
                createUserTable(l);
                loggedInMenu(l);
            }
            else {
                std::cout << "\n\nWrong password or login." << std::endl;
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

            if (registerUser(l, p)) {
                std::cout << "User registered successfully!" << std::endl;
            }
            // Specific messages are now in the function itself
            system("pause");
        }
        else if (authChoice == 3) {
            viewAllUsers();
            system("pause");
        }
        else if (authChoice == 4) {
            deleteUserById();
            system("pause");
        }
        else if (authChoice == 5) {
            return 0;
        }
    }
}
