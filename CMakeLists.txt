cmake_minimum_required(VERSION 3.30)
project(DataBaseApplication)

set(CMAKE_CXX_STANDARD 20)

set(CURL_INCLUDE_DIR "C:/curl")

include_directories(${CURL_INCLUDE_DIR})

add_library(sqlite3 STATIC sqlite3/sqlite3.c)

add_executable(DataBaseApplication main.cpp
        User.h
        User.cpp
        Student.h
        Student.cpp)

target_link_libraries(DataBaseApplication sqlite3)
