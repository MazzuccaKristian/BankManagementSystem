#include <string>

// Params for DB connection
const std::string db_selectedschema = "BankDB";
const std::string db_user = "bankuser";
const std::string db_host_complete = "tcp://127.0.0.1:3306";
const std::string db_password = "password";

// Queries
const std::string login_query = "SELECT Id FROM Users WHERE Username=? AND Password=?";
const std::string registration_query = "INSERT INTO Users(Username, Password) VALUES (?,?)";
const std::string showBalance_query = "SELECT Balance FROM Users WHERE Id=?";
const std::string updateBalance_query = "UPDATE Users SET Balance=? WHERE Id=?";
