#include "Main.h"

int main(){
    bool isStillWorking = true;
    int userChoice;
    int userID = -1;
    bool isUserLogged = false;

    sql::Driver *db_driver;
    sql::Connection *db_connection;
    db_driver = get_driver_instance();
    db_connection = db_driver -> connect(db_host_complete, db_user, db_password);

    cout << "Connecting. Please, wait..." << endl;
    DBSetup(db_connection);
    if(!db_connection -> isValid()){
        perror("ERROR: can't connect to database");
        return 1;
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
                    cout << "Welcome back!" << endl;
                }else{
                    //user's logged in and his account already exists.
                    // OpenNewAccount();
                }
                break;

            case 2:
                ShowBalance(userID, db_connection);
        }
    }while(isStillWorking);

    return 0;
}

