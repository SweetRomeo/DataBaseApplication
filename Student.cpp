//
// Created by berke on 5/16/2025.
//
#include "Student.h"

#include <iostream>
#include <ostream>

Student::Student(const std::string &id, const std::string &name, const std::string &surname, const std::string &email,
                 const std::string &password, const int &age): User(id, name, surname, email, password) {
    this->age = age;
    rc = sqlite3_open("students.db", &db);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Can't open database\n";
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
                                      "AGE INT NOT NULL)";

    rc = sqlite3_exec(db, insertSQL.c_str(), nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cout << "Failed to create table : " << sqlite3_errmsg(db) << '\n';
    }
}

void Student::insertUser() {
    std::cout << "Student::insertUser()" << "\n";
    sqlite3_stmt *stmt = nullptr;
    const std::string insertSQL = "INSERT INTO STUDENT (ID, NAME, SURNAME, EMAIL, PASSWORD, AGE) VALUES (?, ?, ?, ?, ?, ?);";

    rc = sqlite3_prepare_v2(db, insertSQL.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        exit(EXIT_FAILURE);
    }

    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, surname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, age);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

void Student::deleteUser(const std::string& id) {
    std::cout << "Student::deleteUser() - Deleting student with ID: " << id << "\n";
    sqlite3_stmt *stmt = nullptr;
    const std::string deleteSQL = "DELETE FROM STUDENT WHERE ID = ?;";

    // Transaction başlat
    int rc = sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Transaction begin error: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Prepare statement
    rc = sqlite3_prepare_v2(db, deleteSQL.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, nullptr);
        return;
    }

    // Bind parameters
    rc = sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL bind error: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, nullptr);
        return;
    }

    // Execute statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "SQL execution error: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, nullptr);
        return;
    }

    // Check affected rows
    int changes = sqlite3_changes(db);
    if (changes == 0) {
        std::cout << "No student found with ID: " << id << "\n";
    } else {
        std::cout << "Successfully deleted student with ID: " << id << " ("
                  << changes << " row(s) affected)\n";
    }

    // Clean up
    sqlite3_finalize(stmt);

    // Transaction commit
    rc = sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Commit error: " << sqlite3_errmsg(db) << std::endl;
    }
}