cmake_minimum_required(VERSION 3.5)

project(unique_words LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(unique_words src/main.cpp)
