//
// Created by berke on 4/27/2025.
//

#ifndef USER_H
#define USER_H
#include <map>
#include <string>
#include "curl/curl.h"

class User {
public:
    User(const std::string &id, const std::string &name, const std::string &email, const std::string &password)
        : id(id),
          name(name),
          email(email),
          password(password) {
    }

private:
    std::map<std::string, std::string> UserData;
    std::string id;
    std::string name;
    std::string surname;
    std::string email;
    std::string password;
public:
    std::string generateID(const size_t &length);
    void setId(const std::string &id);
    std::string getId();
    void setName(const std::string &name);
    std::string getName();
    void setEmail(const std::string &email);
    std::string getEmail();
    static void sendEmail(const std::string &to, const std::string &message);
    bool isValidEmail(const std::string &email);
};

#endif //USER_H
