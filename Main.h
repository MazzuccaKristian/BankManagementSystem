#include <iostream>
#include <string>

//* DB
#include <stdlib.h>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include "Credentials.h"

//* Queries
#define SETUP_QUERY "CREATE TABLE IF NOT EXISTS Users( \
                        Id INT AUTO_INCREMENT PRIMARY KEY, \
                        Username VARCHAR(30) NOT NULL, \
                        Password VARCHAR(30) NOT NULL, \
                        Balance DECIMAL(8,2) DEFAULT 0.0)"

using std::cout;
using std::endl;
using std::cin;
using std::string;

const string db_user = "bankuser";
const string db_host_complete = "tcp://127.0.0.1:3306";
const string db_password = "password";

void ShowMainMenu();
int GetUserChoice();
bool InputValidation(int input);
int StartLoginPhase(sql::Connection *con);
int Login(sql::Connection *con);
void DBSetup(sql::Connection *con);
void Registration(sql::Connection *con);
void ShowBalance(int userId, sql::Connection *con);
void MakeNewDeposit(int userId, double deposit, sql::Connection *con);

void ShowMainMenu(){
    std::cout << "--- MAIN MENU ---" << std::endl;
    std::cout << "1. Open a new account;" << std::endl;
    std::cout << "2. Show balance;" << std::endl;
    std::cout << "3. New deposit;" << std::endl;
    std::cout << "4. Withdraw;" << std::endl;
    std::cout << "0. Exit." << std::endl;
}

/**
 * @brief Get raw input from user.
 * 
 * @return int - validated input.
 */
int GetUserChoice(){
    int input;
    do{
        cout << "Enter your choice: ";
        cin >> input;
    }while(!InputValidation(input));
    return input;
}

/**
 * @brief input validation phase.
 * 
 * @param input raw input from user.
 * @return true if the input is valid.
 * @return false if the input is NOT valid.
 */
bool InputValidation(int input){
    bool isValid = (input < 0 || input > 5) ? false : true;
    if(!isValid){
        cout << "Option not allowed. Please, try again..." << endl;
    }
    return isValid;
}

/**
 * @brief Login/registration phase.
 *        If the user is already registered, starts login phase; if not, starts registration phase.
 * 
 * @return int - userID from database.
 */
int StartLoginPhase(sql::Connection *con){
    bool isOptionValid = true;
    int input;
    int id;
    do{
        cout << "--- MENU ---" << endl;
        cout << "1. Login;" << endl;
        cout << "2. Registration" << endl;
        cout << "Enter your choice: ";
        cin >> input;
        switch(input){
            case 1:
                // Start login phase.
                id = Login(con);
                if(id < 0){
                    isOptionValid = false;
                    cout << "Username or password incorrect. Please, try again..." << endl;
                }else{
                    isOptionValid = true;
                }
                break;


            //TODO: CASE 2 => start registration phase
            case 2:
                Registration(con);
                //TODO: registration OK, show message and go to login.
                cout << "Registration completed! Please, log in..." << endl;
                isOptionValid = false;

            //TODO: default => input NOT correct, ask new params from user and try again.
        }
    }while(!isOptionValid);
    return id;
}

/**
 * @brief Login phase. Retrieve params from user and execute the login query.
 * 
 * @return int - UserID from database.
 */
int Login(sql::Connection *con){
    string username, password;
    int id;
    cin.ignore();
    cout << "Enter your username: ";
    getline(cin, username);
    cout << "Enter your password: ";
    getline(cin, password);
    // connection test
    // TODO: this test can be a function, used many times.
    if(!con -> isValid()){
        // Here if connection's down, attempt reconnection.
        cout << "Reconnecting. Please, wait..." << endl;
        con -> reconnect();
        if(!con -> isValid()){
            // Can't reach DB.
            exit(EXIT_FAILURE);
        }
    }
    try{
        sql::Statement *stmt;
        sql::ResultSet *res;
        //TODO: possible switch to PreparedStatement
        string query = "SELECT Id FROM Users WHERE Username = '" + username + "' AND Password = '" + password + "'";
        stmt = con -> createStatement();
        res = stmt -> executeQuery(query);
        if(res -> next()){
            id = res -> getInt(1);
        }else{
            // Empty resultset -> user not found in DB
            id = -1;
        }
        delete res;
        delete stmt;
    }catch(sql::SQLException e){
        cout << "ERROR (login): " << e.what() << endl;
    }
    return id;
}

/**
 * @brief First connection to DB
 * 
 * @param con SQL connection (MySQL Connector/C++)
 */
void DBSetup(sql::Connection *con){
    try{
        sql::Statement *stmt;
        sql::ResultSet *res;
        stmt = con -> createStatement();
        // stmt -> execute("USE sql4482158");
        con -> setSchema("BankDB");
        stmt -> execute(SETUP_QUERY);
        delete res;
        delete stmt;
    }catch(sql::SQLException e){
        cout << "ERROR: " << e.what() << endl;
    }
}

void Registration(sql::Connection *con){
    string username, password, confirmPassword;
    bool registrationSuccess = false;
    cin.ignore();
    do{
        cout << "Enter your username: ";
        getline(cin, username);
        cout << "Enter your password: ";
        getline(cin, password);
        cout << "Please, confirm your password: ";
        getline(cin, confirmPassword);
        if(password.compare(confirmPassword) == 0){
            // Test DB connection
            // TODO: this test can be a function, used many times.
            if(!con -> isValid()){
                // Here if connection's down, attempt reconnection.
                cout << "Reconnecting. Please, wait..." << endl;
                con -> reconnect();
                if(!con -> isValid()){
                    // Can't reach DB.
                    exit(EXIT_FAILURE);
                }
            }       
            // DB registration phase starts here.
            try{
                sql::Statement *stmt;
                //TODO: possible switch to PreparedStatement
                string query = "INSERT INTO Users(Username, Password) VALUES ('" + username + "', '" + password + "')";
                stmt = con -> createStatement();
                stmt -> execute(query);
                delete stmt;
                registrationSuccess = true;
            }catch(sql::SQLException e){
                cout << "ERROR (registration): " << e.what() << endl;
            }
        }else{
            // password and confirm mismatched. Try again this phase.
            cout << "Your passwords don't match. Please, try again..." << endl;
            registrationSuccess = false;
        }
    }while(!registrationSuccess);
}

void ShowBalance(int userId, sql::Connection *con){
    // TODO: this test can be a function, used many times.
    if(!con -> isValid()){
        // Here if connection's down, attempt reconnection.
        cout << "Reconnecting. Please, wait..." << endl;
        con -> reconnect();
        if(!con -> isValid()){
            // Can't reach DB.
            exit(EXIT_FAILURE);
        }
    } 
    try{
        sql::Statement *stmt;
        sql::ResultSet *result;
        stmt = con -> createStatement();
        result = stmt -> executeQuery("SELECT Balance FROM Users WHERE Id='" + std::to_string(userId) + "'");
        while(result -> next()){
            cout << "Balance: " << result -> getDouble("Balance") << "$" << endl;
        }
        delete stmt;
        delete result;
    }catch(sql::SQLException e){
        cout << "ERROR (registration): " << e.what() << endl;
    }
}

void MakeNewDeposit(int userId, double deposit, sql::Connection *con){
    // TODO: this test can be a function, used many times.
    if(!con -> isValid()){
        // Here if connection's down, attempt reconnection.
        cout << "Reconnecting. Please, wait..." << endl;
        con -> reconnect();
        if(!con -> isValid()){
            // Can't reach DB.
            exit(EXIT_FAILURE);
        }
    }
    try{
        sql::Statement *stmt;
        string query = "UPDATE Users SET Balance = '" + std::to_string(deposit) + "' WHERE ID = '" + std::to_string(userId) + "'";
        stmt = con -> createStatement();
        stmt -> execute(query);
        delete stmt;
    }catch(sql::SQLException e){
        cout << "ERROR (new deposit): " << e.what() << endl;
    }
}