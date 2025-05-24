//
// Created by berke on 5/16/2025.
//
#include "Student.h"

#include <iostream>
#include <ostream>

Student::Student(const std::string &id, const std::string &name, const std::string &surname, const std::string &email,
                 const std::string &password, const std::string &area): User(id, name, surname, email, password) {
    this->area = area;
    rc = sqlite3_open("students.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open database" << std::endl;
        sqlite3_close(db);
        db = nullptr;
        throw std::runtime_error("Can't open database");
    }
}

void Student::createTable()
{
    const std::string& insertSQL = "CREATE TABLE IF NOT EXISTS STUDENT ("
                                      "ID TEXT PRIMARY KEY NOT NULL, "
                                      "NAME TEXT NOT NULL,"
                                      "SURNAME TEXT NOT NULL,"
                                      "EMAIL TEXT NOT NULL UNIQUE,"
                                      "PASSWORD TEXT NOT NULL,"
                                      "AREA TEXT NOT NULL)";

    rc = sqlite3_exec(db, insertSQL.c_str(), nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cout << "Failed to create table : " << sqlite3_errmsg(db) << std::endl;
    }
}

void Student::insertUser() {
    std::cout << "Student::insertUser()" << std::endl;
    sqlite3_stmt *stmt = nullptr;
    const std::string insertSQL = "INSERT INTO STUDENT (ID, NAME, SURNAME, EMAIL, PASSWORD, AREA) VALUES (?, ?, ?, ?, ?, ?);";

    rc = sqlite3_prepare_v2(db, insertSQL.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        exit(EXIT_FAILURE);
    }

    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, surname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, area.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

void Student::deleteUser(std::string &id) {
    std::cout << "Student::deleteUser()" << std::endl;
    sqlite3_stmt *stmt;
    std::string deleteSQL = "DELETE FROM STUDENT WHERE ID = ?";
    rc = sqlite3_prepare_v2(db, deleteSQL.c_str(), -1, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);

    std::cout << "User with this id : " << id << "has been deleted\n";
}