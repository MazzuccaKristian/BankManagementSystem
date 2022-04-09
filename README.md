# Bank Management System
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white) ![MySQL](https://img.shields.io/badge/mysql-%2300f.svg?style=for-the-badge&logo=mysql&logoColor=white)
![Github Version](https://img.shields.io/badge/version-1.0.0-brightgreen) ![Generic badge](https://img.shields.io/badge/status-completed-brightgreen)

## Simple bank management system
Manage your bank account with this simple console app.
Project idea from [InterviewBit](https://www.interviewbit.com/blog/cpp-projects/), "Bank Management System In C++".

## Features

- Log in your  bank account
- Register a new account
- Open a new bank account
- Review your balance
- Make a new deposit
- Withdraw from your balance

## Tech

"Bank Management System" uses:
- [C++11](https://isocpp.org/)
- [MySQL](https://dev.mysql.com/doc/)
- [MySQL Connector/C++](https://dev.mysql.com/doc/connector-cpp/8.0/en/)

## Installation
1. Clone the repo.
2. Install a local [MySQL server](https://dev.mysql.com/downloads/).
3. Install [MySQL Connector/C++](https://dev.mysql.com/doc/connector-cpp/8.0/en/).
4. Start MySQL server: `sudo service mysql start`
5. Setup MySQL server:
   ```sql
   -- Create 'BankDB' database
   CREATE DATABASE IF NOT EXISTS BankDB;
   -- Create 'Users' table
   CREATE TABLE IF NOT EXISTS Users(
        Id INT AUTO_INCREMENT PRIMARY KEY,
        Username VARCHAR(30) NOT NULL,
        Password VARCHAR(30) NOT NULL,
        Balance DECIMAL(8,2) DEFAULT 0.0);
   -- Create 'bankuser' user
   CREATE USER 'bankuser'@'localhost' IDENTIFIED WITH mysql_native_password BY 'password';
   -- Grant privileges to bankuser
   GRANT PRIVILEGES SELECT, INSERT, UPDATE ON BankDB.Users TO 'bankuser'@'localhost';
   ```

6. Compile code: `g++ -std=c++11 -o Main Main.cpp -lmysqlcppconn`
7. You're ready to go! Run the program: `./Main`