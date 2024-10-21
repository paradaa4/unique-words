add_library(core)

target_sources(core
  PRIVATE
    FileReader.cpp
    FileReader.h
)

target_include_directories(core
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
  )
