# Bank Management System
![Github Version](https://img.shields.io/badge/version-0.2.1-brightgreen) ![Generic badge](https://img.shields.io/badge/status-ongoing-brightgreen.svg)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white) ![MySQL](https://img.shields.io/badge/mysql-%2300f.svg?style=for-the-badge&logo=mysql&logoColor=white)

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
BankManagementSystem requires ... to run.
1. Clone the repo.
2. Install a local [MySQL server](https://dev.mysql.com/downloads/).
3. Install [MySQL Connector/C++](https://dev.mysql.com/doc/connector-cpp/8.0/en/).
4. Setup MySQL server:
   ```sh
   # Start the server
   sudo service mysql start
   # Create 'BankDB' database
   mysql> CREATE DATABASE IF NOT EXISTS BankDB;
   # Create 'Users' table
   mysql> CREATE TABLE IF NOT EXISTS Users(
            Id INT AUTO_INCREMENT PRIMARY KEY,
            Username VARCHAR(30) NOT NULL,
            Password VARCHAR(30) NOT NULL,
            Balance DECIMAL(8,2) DEFAULT 0.0);
   ```
5. Create user 'bankuser':
   ```sh
   mysql> CREATE USER 'bankuser'@'localhost' IDENTIFIED WITH mysql_native_password BY 'password';
   mysql> GRANT PRIVILEGES CREATE, SELECT, INSERT, DELETE ON BankDB.Users TO 'bankuser'@'localhost';
   ```
6. Compile code:
   ```sh
   g++ -std=c++11 -o Main Main.cpp - lmysqlcppconn
   ```