//
// Created by berke on 8/25/2025.
//

#ifndef TEACHER_H
#define TEACHER_H
#include "User.h"

class Teacher : public User {
    public:
    Teacher() = default;
    void createTable() override;
    void insertUser() override;
    void deleteUser(const std::string &id) override;
    Teacher(const std::string &id, const std::string &name, const std::string &surname, const std::string &email, const std::string &password, std::string phoneNumber, std::string  area);
    ~Teacher()final {
        if (db) {
            sqlite3_close(db);
        }
    }
private:
    std::string m_phoneNumber;
    std::string m_area;
};

#endif //TEACHER_H
