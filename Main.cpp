#include "Main.h"

int main(){
    bool isStillWorking = true;
    int userChoice;
    int userID = -1; // default value, MUST be updated with login/registration.
    bool isUserLogged = false;

    // DB connection
    sql::Driver *db_driver;
    sql::Connection *db_connection;
    try{
        db_driver = get_driver_instance();
        cout << "Connecting...";
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
                //TODO: possible check on amount (positive-only? non-zero?)
                double deposit;
                cin >> deposit;
                MakeNewDeposit(userID, deposit, db_connection);
                break;

            case 4:
                if(!isUserLogged){
                    userID = StartLoginPhase(db_connection);
                    isUserLogged = true;
                }
                cout << "Enter the amount you want to withdraw: ";
                double amount;
                cin >> amount;
                Withdraw(userID, amount, db_connection);
                break;
        }
    }while(isStillWorking);

    return EXIT_SUCCESS;
}

