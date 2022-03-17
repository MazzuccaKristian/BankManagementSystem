#include <iostream>
#include <fstream>

#define BANK "bank.txt"
#define USERS "users.txt"

void Setup();
int GetUserInput();
bool VerifyChoice(int input);
void ShowMainMenu();

/**
 * @brief Environment setup: check archives.
 * 
 */
void Setup(){
    std::ifstream bank;
    std::ifstream users;
    bank.open(BANK);
    if(!bank.is_open()){
        std::ofstream newBank;
        newBank.open(BANK);
        newBank.close();
    }else{
        bank.close();
    }
    users.open(USERS);
    if(!users.is_open()){
        std::ofstream newUSers;
        newUSers.open(USERS);
        newUSers.close();
    }else{
        users.close();
    }
}

/**
 * @brief Get input from keyboard.
 * 
 * @return int verified choice.
 */
int GetUserInput(){
    int input;
    do{
        std::cout << "Enter your option: ";
        std::cin >> input;
    }while(!VerifyChoice(input));
    return input;
}

/**
 * @brief Check user input.
 * 
 * @param input raw input.
 * @return true if the input is valid
 * @return false if the input is NOT valid.
 */
bool VerifyChoice(int input){
    bool isValid = false;
    if(input >= 0 && input <= 5){
        isValid = true;
    }else{
        std::cout << "Option not allowed. Please, try again..." << std::endl;
    }
    return isValid;
}

void ShowMainMenu(){
    std::cout << "--- MAIN MENU ---" << std::endl;
    std::cout << "1. Open a new account;" << std::endl;
    std::cout << "2. Show balance;" << std::endl;
    std::cout << "3. New deposit;" << std::endl;
    std::cout << "4. Withdraw;" << std::endl;
    std::cout << "5. Exit." << std::endl;
}