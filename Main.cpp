#include "Main.h"

int main(){
    bool isStillWorking = true;
    int userChoice;
    int userID = -1; // default value, MUST be updated with login/registration.
    bool isUserLogged = false;
    string rawInput;

    // DB connection
    sql::Driver *db_driver;
    sql::Connection *db_connection;
    try{
        cout << "Connecting...";
        db_driver = get_driver_instance();
        db_connection = db_driver -> connect(db_host_complete, db_user, db_password);
        if(db_connection -> isValid()){
            db_connection -> setSchema(db_selectedschema);
            cout << " OK" << endl;
        }
    }catch(sql::SQLException e){
        perror(e.what());
        exit(EXIT_FAILURE);
    }

    do{
        ShowMainMenu();
        userChoice = GetUserChoice();
        switch(userChoice){
            case 0:
                cout << "Program ended. Please, wait..." << endl;
                delete db_connection;
                isStillWorking = false;
                break;

            case 1:
                if(!isUserLogged){
                    userID = StartLoginPhase(db_connection);
                    isUserLogged = true;
                    cout << "Welcome back!" << endl;
                }
                break;

            case 2:
                if(!isUserLogged){
                    userID = StartLoginPhase(db_connection);
                    isUserLogged = true;
                }
                ShowBalance(userID, db_connection);
                break;

            case 3:
                if(!isUserLogged){
                    userID = StartLoginPhase(db_connection);
                    isUserLogged = true;
                }
                cout << "Enter the amount you want to deposit: ";
                double deposit;
                getline(cin, rawInput);
                try{
                    deposit = std::stod(rawInput);
                }catch(std::invalid_argument excpetion){
                    deposit = 0.0;
                }
                if(deposit < 0){
                    deposit = -deposit;
                }
                UpdateBalance(db_connection, userID, deposit);
                // MakeNewDeposit(userID, deposit, db_connection);
                break;

            case 4:
                if(!isUserLogged){
                    userID = StartLoginPhase(db_connection);
                    isUserLogged = true;
                }
                cout << "Enter the amount you want to withdraw: ";
                double amount;
                getline(cin, rawInput);
                try{
                    amount = std::stod(rawInput);
                }catch(std::invalid_argument exception){
                    amount = 0.0;
                }
                if(amount > 0){
                    amount = -amount;
                }
                UpdateBalance(db_connection, userID, amount);
                break;
        }
    }while(isStillWorking);

    return EXIT_SUCCESS;
}

