//
// Created by berke on 4/28/2025.
//
#include "User.h"
#include <regex>
#include <random>
#include <iostream>

static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp) {
    std::string *data = static_cast<std::string*>(userp);
    if(size*nmemb < 1 || data->empty()) return 0;

    size_t len = data->copy(static_cast<char*>(ptr), size*nmemb);
    *data = data->substr(len);
    return len;
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

User::User(const std::string &id, const std::string &name, const std::string &surname, const std::string &email, const std::string &password)
    : id(id), name(name), surname(surname), email(email), password(password), db(nullptr) {

    rc = sqlite3_open("users.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Veritabanı açılamadı: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db); // Hata durumunda db'yi kapat
        db = nullptr; // db pointer'ını null yap
        throw std::runtime_error("Veritabanı bağlantısı kurulamadı");
    }

   /* try {
        createTable();
        insertUser();
    } catch (const std::exception& e) {
        sqlite3_close(db);
        db = nullptr;
        throw;
    }*/
}

void User::createTable() {
    const auto createTableSQL = "CREATE TABLE IF NOT EXISTS USER("
                                    "ID TEXT PRIMARY KEY NOT NULL,"
                                    "NAME TEXT NOT NULL,"
                                    "SURNAME TEXT NOT NULL,"
                                    "EMAIL TEXT NOT NULL UNIQUE,"
                                    "PASSWORD TEXT NOT NULL);";

    rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Tablo oluşturulamadı: " << sqlite3_errmsg(db) << std::endl;
    }
}

void User::insertUser() {
    sqlite3_stmt* stmt;
    const char* insertSQL = "INSERT INTO USER (ID, NAME, SURNAME, EMAIL, PASSWORD) VALUES (?, ?, ?, ?, ?);";

    rc = sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Sorgu hazırlanamadı: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, surname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, password.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to while adding user: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

std::string User::getId() {
    return id;
}

void User::setUser(const std::string &newId, const std::string &newName,
                  const std::string &newSurname, const std::string &newEmail,
                  const std::string &newPassword) {
    // Önce email geçerli mi kontrol et
    if (!isValidEmail(newEmail)) {
        throw std::invalid_argument("Invalid email format!");
    }

    // ID değişmediyse doğrudan güncelle
    if (newId == id) {
        id = newId;
        name = newName;
        surname = newSurname;
        email = newEmail;
        password = newPassword;
        updateUserInDB();
        return;
    }

    // Yeni ID için kontrol yap
    sqlite3_stmt* stmt;
    const char* checkSQL = "SELECT 1 FROM USER WHERE ID = ? LIMIT 1;";

    if (sqlite3_prepare_v2(db, checkSQL, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, newId.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) { // Kullanıcı varsa
            sqlite3_finalize(stmt);
            throw std::invalid_argument("Bu ID zaten kullanımda!");
        }
        sqlite3_finalize(stmt);
    }

    // Önce eski kaydı sil
    const char* deleteSQL = "DELETE FROM USER WHERE ID = ?;";
    if (sqlite3_prepare_v2(db, deleteSQL, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    // Yeni değerleri ata
    id = newId;
    name = newName;
    surname = newSurname;
    email = newEmail;
    password = newPassword;

    // Veritabanına yeni kaydı ekle
    insertUser();
}

std::string User::getName() {
    return name;
}

std::string User::generateID(const size_t& length) {
    static const std::string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, characters.size() - 1);

    std::string id;
    id.reserve(length);
    for(size_t i = 0; i < length; i++) {
        id += characters[dis(gen)];
    }
    return id;
}


/*
void User::sendEmail(const std::string &to, const std::string &message) {
    CURL *curl = curl_easy_init();
    if(curl) {
        struct curl_slist *recipients = nullptr;
        recipients = curl_slist_append(recipients, to.c_str());

        std::string email_body = "To: " + to + "\r\n"
                              "From: noreply@engrare.com\r\n"
                              "Subject: Verification\r\n\r\n"
                              "Doğrulama linki: https://example.com/verify?token=" + message;

        curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.example.com");
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, "noreply@engrare.com");
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
        curl_easy_setopt(curl, CURLOPT_USERPWD, "user:password");
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
        curl_easy_setopt(curl, CURLOPT_READDATA, &email_body);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        CURLcode res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            std::cerr << "Email gönderilemedi: " << curl_easy_strerror(res) << std::endl;
        }

        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
    }
}*/


bool User::isValidEmail(const std::string &email) {
    const std::regex pattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return std::regex_match(email, pattern);
}

void User::updateUserInDB() {
    sqlite3_stmt* stmt;
    const char* updateSQL = "UPDATE USER SET NAME = ?, SURNAME = ?, EMAIL = ?, PASSWORD = ? WHERE ID = ?;";

    rc = sqlite3_prepare_v2(db, updateSQL, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Update sorgusu hazırlanamadı: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Parametreleri bağla (ID en sona)
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, surname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, id.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Kullanıcı güncellenemedi: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Kullanıcı başarıyla güncellendi!" << std::endl;
    }

    sqlite3_finalize(stmt);
}