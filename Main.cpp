#include "Main.h"

int main(){
    bool isStillWorking = true;
    int choice;

    Setup();

    do{
        ShowMainMenu();
        choice = GetUserInput();
        switch(choice){
            case 1:
                OpenAccount();
                break;

            case 2:
                ShowBalance();
                break;

            case 3:
                NewDeposit();
                break;

            case 4:
                Withdraw();
                break;

            case 5:
                Exit();
        }
    }while(isStillWorking);
    return 0;
}

