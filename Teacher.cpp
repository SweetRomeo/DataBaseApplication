//
// Created by berke on 8/25/2025.
//
#include "Teacher.h"

#include <iostream>
#include <ostream>
#include <sstream>
#include <utility>
#include <windows.h>

Teacher::Teacher(const std::string &id, const std::string &name, const std::string &surname, const std::string &email,
                 const std::string &password, std::string phoneNumber, std::string area)
    : User(id, name, surname, email, password), m_area(std::move(area)), m_phoneNumber(std::move(phoneNumber)) {
}

void Teacher::createTable() {
    const std::string& createSQL = "CREATE TABLE IF NOT EXISTS teacher ("
                                      "ID TEXT PRIMARY KEY NOT NULL,"
                                      "NAME TEXT NOT NULL,"
                                      "SURNAME TEXT NOT NULL,"
                                      "EMAIL TEXT NOT NULL,"
                                      "PASSWORD TEXT NOT NULL,"
                                      "AGE INT NOT NULL,"
                                      "PHONE TEXT NOT NULL,"
                                      "AREA TEXT NOT NULL)";
    rc = sqlite3_exec(db, createSQL.c_str(), nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << '\n';
    }
    else {
        std::cout << "Table created successfully.\n";
    }
}

void Teacher::insertUser() {
    sqlite3_stmt *stmt = nullptr;
    const std::string& insertSQL = "INSERT INTO TEACHER (ID, NAME, SURNAME, EMAIL, PASSWORD, AGE, PHONE, AREA) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";

    rc = sqlite3_prepare_v2(db, insertSQL.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << '\n';
        exit(EXIT_FAILURE);
    }

    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, surname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, age);
    sqlite3_bind_text(stmt, 7, m_phoneNumber.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 8, m_area.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << '\n';
    }
    sqlite3_finalize(stmt);
}


void Teacher::deleteUser(const std::string &id) {
    sqlite3_stmt *stmt = nullptr;
    std::string deleteSQL = "DELETE FROM TEACHER WHERE ID = ?";

    int rc = sqlite3_prepare_v2(db, deleteSQL.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << '\n';
        sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, nullptr);
        return;
    }

    rc = sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << '\n';
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, nullptr);
        return;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << '\n';
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, nullptr);
        return;
    }

    int changes = sqlite3_changes(db);
    std::stringstream resultText;
    if (changes == 0) {
        resultText << "No student found with ID : " << id;
    }
    else {
        resultText << "Successfully deleted student with ID : " << id << " ("
                   << changes << "row(s) affected)\n";
    }

    sqlite3_finalize(stmt);

    rc = sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << '\n';
    }
}