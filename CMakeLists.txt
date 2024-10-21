cmake_minimum_required(VERSION 3.5)

project(unique_words LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory(src)
    
enable_testing()
add_subdirectory(test)    