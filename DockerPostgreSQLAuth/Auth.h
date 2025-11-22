#pragma once
#include <string>

void initialize_database();
bool registerUser(const std::string& name, const std::string& pass);
bool loginUser(const std::string& name, const std::string& pass);
void viewAllUsers();
void deleteUserById();

// The functionality that was inside the logged-in user loop
void addElementToUserTable(const std::string& username, const std::string& data);
void createUserTable(const std::string& username);
