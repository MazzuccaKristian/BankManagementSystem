#include <iostream>
#include <string>

//* DB
#include <stdlib.h>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "DB_utils.h"

using std::cout;
using std::endl;
using std::cin;
using std::string;

void ShowMainMenu();
int GetUserChoice();
bool InputValidation(int input);
int StartLoginPhase(sql::Connection *con);
int Login(sql::Connection *con);
void Registration(sql::Connection *con);
void ShowBalance(int userId, sql::Connection *con);
void MakeNewDeposit(int userId, double deposit, sql::Connection *con);
void Withdarw(int userId, double amount, sql::Connection *con);
void UpdateBalance(sql::Connection *con,int userId, double amount);

void ShowMainMenu(){
    std::cout << "--- MAIN MENU ---" << std::endl;
    std::cout << "1. Open a new account;" << std::endl;
    std::cout << "2. Show balance;" << std::endl;
    std::cout << "3. New deposit;" << std::endl;
    std::cout << "4. Withdraw;" << std::endl;
    std::cout << "0. Exit." << std::endl;
}

/**
 * @brief Retrieve input from user.
 * 
 * @return int - validated input.
 */
int GetUserChoice(){
    string rawInput;
    int input;
    do{
        cout << "Enter your choice: ";
        getline(cin, rawInput);
        try{
            input = std::stoi(rawInput);      
        }catch(std::invalid_argument exception){
            input = -1; // brute-force invalid value
        }
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
    string rawInput;
    int id;
    do{
    int input;
        cout << "--- LOGIN MENU ---" << endl;
        cout << "1. Login;" << endl;
        cout << "2. Registration" << endl;
        cout << "Enter your choice: ";
        getline(cin, rawInput);
        try{
            input = std::stoi(rawInput);
        }catch(std::invalid_argument exception){
            input = -1; //brute-force invalid value.
        }
        switch(input){
            case 1:
                id = Login(con);
                if(id < 0){
                    isOptionValid = false;
                    cout << "Username or password incorrect. Please, try again..." << endl;
                }else{
                    isOptionValid = true;
                }
                break;

            case 2:
                Registration(con);
                cout << "Registration completed! Please, log in..." << endl;
                isOptionValid = false;
                break;

            default:
                cout << "Option not allowed. Try again..." << endl;
                isOptionValid = false;
                break;
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
    if(!con -> isValid()){
        // DB can't be reached.
        cout << "ERROR (login): can't connect to DB." << endl;
        exit(EXIT_FAILURE);
    }
    cout << "Enter your username: ";
    getline(cin, username);
    cout << "Enter your password: ";
    getline(cin, password);
    try{
        sql::PreparedStatement *p_stmt;
        sql::ResultSet *res;
        p_stmt = con -> prepareStatement(login_query);
        p_stmt -> setString(1, username);
        p_stmt -> setString(2, password);
        res = p_stmt -> executeQuery();
        if(res -> next()){
            id = res -> getInt(1);
        }else{
            id = -1;
        }
        delete p_stmt;
        delete res;
    }catch(sql::SQLException e){
        cout << "ERROR (login): " << e.what() << endl;
    }
    return id;
}

void Registration(sql::Connection *con){
    string username, password, confirmPassword;
    bool registrationSuccess = false;
    if(!con -> isValid()){
        cout << "ERROR (registration): can't connect to DB" << endl;
        exit(EXIT_FAILURE);
    } 
    do{
        cout << "Enter your username: ";
        getline(cin, username);
        cout << "Enter your password: ";
        getline(cin, password);
        cout << "Please, confirm your password: ";
        getline(cin, confirmPassword);
        if(password.compare(confirmPassword) == 0){
            // DB registration phase starts here.
            try{
                sql::PreparedStatement *p_stmt;
                p_stmt = con -> prepareStatement(registration_query);
                p_stmt -> setString(1, username);
                p_stmt -> setString(2, password);
                p_stmt -> executeQuery();
                delete p_stmt;
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
    if(!con -> isValid()){
        // DB can't be reached.
        cout << "ERROR (show balance): can't connect to DB." << endl;
        exit(EXIT_FAILURE);
    } 
    try{
        sql::PreparedStatement *p_stmt;
        sql::ResultSet *res;
        p_stmt = con -> prepareStatement(showBalance_query);
        p_stmt -> setInt(1, userId);
        res = p_stmt -> executeQuery();
        while(res -> next()){
            cout << "Balance: " << res -> getDouble("Balance") << "$" << endl;
        }
        delete p_stmt;
        delete res;
    }catch(sql::SQLException e){
        cout << "ERROR (registration): " << e.what() << endl;
    }
}

void MakeNewDeposit(int userId, double deposit, sql::Connection *con){
    double originalBalance, newBalance;
    if(deposit < 0){
        deposit = -deposit;
    }
    if(!con -> isValid()){
        cout << "ERROR (new deposit): can't connect to DB" << endl;
        exit(EXIT_FAILURE);
    }
    UpdateBalance(con, userId, deposit);
}

void Withdraw(int userId, double amount, sql::Connection *con){
    double originalBalance, newBalance;
    if(amount > 0){
        amount = -amount;
    }
    if(!con -> isValid()){
        cout << "ERROR (withdraw): can't connect to DB" << endl;
        exit(EXIT_FAILURE);
    }
    UpdateBalance(con, userId, amount);
}

void UpdateBalance(sql::Connection *con,int userId, double amount){
    double originalBalance, newBalance;
    if(!con -> isValid()){
        cout << "ERROR (update balance): can't connect to DB" << endl;
        exit(EXIT_FAILURE);
    }
    try{
        sql::PreparedStatement *p_stmt;
        sql::ResultSet *res;
        // 1. Retrieve user's balance.
        p_stmt = con -> prepareStatement(showBalance_query);
        p_stmt -> setInt(1, userId);
        res = p_stmt -> executeQuery();
        while(res -> next()){
            originalBalance = res -> getDouble("Balance");
        }
        delete res;
        newBalance = originalBalance + amount;
        // 2. Update user's balance
        p_stmt = con -> prepareStatement(updateBalance_query);
        p_stmt -> setDouble(1, newBalance);
        p_stmt -> setInt(2, userId);
        p_stmt -> executeQuery();
        cout << "Operation completed!" << endl;
        delete p_stmt;
    }catch(sql::SQLException e){
        cout << "ERROR (withdraw): " << e.what() << endl;
    }
}