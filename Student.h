//
// Created by berke on 5/10/2025.
//

#ifndef STUDENT_H
#define STUDENT_H
#include "sqlite3/sqlite3.h"
#include "User.h"

class Student : public User {
public:
    Student() = delete;
    Student(const std::string &id, const std::string &name, const std::string &surname, const std::string &email,
            const std::string &password, const std::string &area);
    ~Student() {
        if (!db) {
            sqlite3_close(db);
        }
    }
private:
    std::string area;
public:
    void createTable()override;
    void insertUser()override;
    void deleteUser(std::string& id)override;
};

#endif //STUDENT_H
