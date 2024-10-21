include(FetchContent)
FetchContent_Declare(
  gtest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gtest)

function(add_unit_test name)
  add_executable(${name}-test ${name}_unittest.cpp)
  target_link_libraries(${name}-test
    PUBLIC
      core
      gtest_main
  )
  add_test(NAME ${name}-test COMMAND ${name}-test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test/)

endfunction()

add_unit_test(FileReader)
add_unit_test(StringSplitter)
