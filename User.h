//
// Created by berke on 4/27/2025.
//

#ifndef USER_H
#define USER_H
#include <map>
#include <string>
/*#include "curl/curl.h"*/
#include "sqlite3/sqlite3.h"

class User {
public:
    User() = delete;
    User(const std::string &id, const std::string &name, const std::string &surname, const std::string &email, const std::string &password);
    virtual ~User() {
        if (db) {
            sqlite3_close(db);
        }
    }

protected:
    sqlite3 *db{};
    char* zErrMsg = nullptr;
    int rc;
    std::map<std::string, std::string> UserData;
    std::string id;
    std::string name;
    std::string surname;
    std::string email;
    std::string password;

    void updateUserInDB();
public:
    virtual void createTable();
    virtual void insertUser();
    virtual void deleteUser(std::string& id) = 0;
    static std::string generateID(const size_t &length);
    void setId(const std::string &id);
    std::string getId();
    void setName(const std::string &name);
    std::string getName();
    void setEmail(const std::string &email);
    std::string getEmail();
    void setUser(const std::string &id, const std::string &name, const std::string& surname,
        const std::string &email, const std::string &password);
    static void sendEmail(const std::string &to, const std::string &message);
    bool isValidEmail(const std::string &email);
};

#endif //USER_H
