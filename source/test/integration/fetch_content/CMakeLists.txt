cmake_minimum_required(VERSION 3.16)
set(NAME coro-integration-fetch-content)
project(${NAME} CXX)

include(FetchContent)
FetchContent_Declare(
  coro
  GIT_REPOSITORY https://github.com/automainint/coro.git
  GIT_TAG dev)
set(CORO_ENABLE_TESTING OFF)
FetchContent_MakeAvailable(coro)

add_executable(${NAME})
target_sources(${NAME} PRIVATE main.cpp)
target_link_libraries(${NAME} PRIVATE coro::coro)

enable_testing()
add_test(NAME ${NAME}-run COMMAND ${NAME})
set_tests_properties(${NAME}-run PROPERTIES TIMEOUT "15")
