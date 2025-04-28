//
// Created by berke on 4/28/2025.
//
#include "User.h"
#include <regex>
#include <random>

std::string User::getId() {
    return id;
}

void User::setId(const std::string& id) {

}

std::string User::getName() {
    return name;
}

std::string User::generateID(const size_t& length) {
    std::string characters = "ABCDEFGHIJKLMNQPRSTUOVWXYZ123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sizeof(characters) - 1);
    std::string id = "";
    for (size_t i = 0; i < length; i++) {
        id += characters[dis(gen)];
    }
    return id;
}


void User::sendEmail(const std::string &to, const std::string &message) {
    CURL *curl = curl_easy_init();
    if (curl != nullptr) {
        curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.example.com");
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, "noreply@engrare.com");
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, to);
        curl_easy_setopt(curl, CURLOPT_USERPWD, "user:password");
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);

        std::string email_body = "Doğrulama linki: https://example.com/verify?token=" + message;
        curl_easy_setopt(curl, CURLOPT_READDATA, &email_body);

        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
}


bool User::isValidEmail(const std::string &email) {
    const std::regex pattern(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    return std::regex_match(email, pattern);
}