#include <iostream>
#include <string>

#include "Student.h"
#include "User.h"
#include "sqlite3/sqlite3.h"
/*#include "curl/curl.h"*/

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int main() {

    User* user1 = new Student("g12j45l89", "Mustafa", "Say", "mustafasay@gmail.com", "mustafa123", "Software");
    user1->createTable();
    user1->insertUser();
    //user1->deleteUser((std::string&)"123456");
    //user1.createTable();
    //user1.insertUser();
    /*sqlite3 *db;
    char* zErrMsg = nullptr;
    int rc;
    std::string sql;

    rc = sqlite3_open("test.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }
    fprintf(stderr, "Opened database successfully\n");

    sql = "CREATE TABLE COMPANY("  \
      "ID INT PRIMARY KEY     NOT NULL," \
      "NAME           TEXT    NOT NULL," \
      "AGE            INT     NOT NULL," \
      "ADDRESS        CHAR(50)," \
      "SALARY         REAL );";

    rc = sqlite3_exec(db, sql.c_str(), callback, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    fprintf(stderr, "Create table complete\n");
    /* Create SQL statement #1#
    sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
          "VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
          "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
          "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
          "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
          "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
          "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
          "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";
    sqlite3_exec(db, sql.c_str(), callback, nullptr, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stderr, "Insert completed\n");
    }
    sqlite3_close(db);*/
    return 0;
}