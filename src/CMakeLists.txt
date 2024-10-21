cmake_minimum_required(VERSION 3.5)

add_subdirectory(core)

add_executable(unique_words 
    main.cpp
)

target_link_libraries(unique_words
  PRIVATE
    core
)
