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

void ShowMainMenu();
int GetUserChoice();
bool InputValidation(int input);
int LoginUser(sql::Connection *con);
int Login(sql::Connection *con);
void DBSetup(sql::Connection *con);

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
int LoginUser(sql::Connection *con){
    bool isOptionValid = true;
    int input;
    int id;
    do{
        cout << "--- MENU ---" << endl;
        cout << "1. Login;" << endl;
        cout << "2. Registration" << endl;
        cout << "Enter your choice: " << endl;
        cin >> input;
        switch(input){
            case 1:
                // Start login phase.
                id = Login(con);
                if(id < 0){
                    // User not found in DB, try again.
                    continue;
                }

            //TODO: CASE 2 => start registration phase

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
            cout << "Empty resultset, user not found" << endl;
            id = -1;
        }
    }catch(sql::SQLException e){
        cout << "ERROR: " << e.what() << endl;
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
        stmt -> execute("USE sql4482158");
        stmt -> execute(SETUP_QUERY);
        delete res;
        delete stmt;
    }catch(sql::SQLException e){
        cout << "ERROR: " << e.what() << endl;
    }
}