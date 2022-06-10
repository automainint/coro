cmake_minimum_required(VERSION 3.16)

option(CORO_ENABLE_TESTING "Enable testing" ON)

set(CORO_PROJECT coro)
set(CORO_LIBRARY coro)
set(CORO_TEST coro-test)
set(CORO_UNITTESTS coro-unittests)
set(CORO_TARGETS coro-targets)

if(NOT DEFINED CMAKE_BUILD_PARALLEL_LEVEL)
  set(CMAKE_BUILD_PARALLEL_LEVEL 3)
endif()

project(
  ${CORO_PROJECT}
  VERSION 0.1.1
  DESCRIPTION "Coroutine library"
  LANGUAGES CXX)

add_library(${CORO_LIBRARY} STATIC)
add_library(${CORO_PROJECT}::${CORO_LIBRARY} ALIAS ${CORO_LIBRARY})
target_include_directories(
  ${CORO_LIBRARY} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source>)
target_compile_features(${CORO_LIBRARY} PUBLIC cxx_std_20)

enable_testing()

if(CORO_ENABLE_TESTING)
  find_package(Catch2 QUIET)

  if(NOT Catch2_FOUND)
    include(FetchContent)

    FetchContent_Declare(
      Catch2
      GIT_REPOSITORY https://github.com/catchorg/Catch2.git
      GIT_TAG v2.13.9)

    FetchContent_MakeAvailable(Catch2)
  endif()

  add_executable(${CORO_TEST})

  target_link_libraries(
    ${CORO_TEST}
    PRIVATE
    ${CORO_LIBRARY} Catch2::Catch2)

  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(
      ${CORO_LIBRARY} PUBLIC
      -fsanitize=undefined,address
      --coverage -O0 -g)
    target_link_options(
      ${CORO_LIBRARY} PUBLIC
      -fsanitize=undefined,address
      --coverage)
  endif()

  add_test(
    NAME ${CORO_UNITTESTS}
    COMMAND ${CORO_TEST})

  set_tests_properties(
    ${CORO_UNITTESTS}
    PROPERTIES
    TIMEOUT "15")
endif()

add_subdirectory(source)

include(GNUInstallDirs)

file(
  GLOB_RECURSE headers_
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/source/coro
  ${CMAKE_CURRENT_SOURCE_DIR}/source/coro/*.h)

foreach(path_ ${headers_})
  get_filename_component(dir_ "${path_}" DIRECTORY)

  install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/source/coro/${path_}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/coro/${dir_})
endforeach()

unset(headers_)
unset(path_)
unset(dir_)

install(
  TARGETS ${CORO_LIBRARY}
  EXPORT ${CORO_TARGETS}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
  EXPORT ${CORO_TARGETS}
  FILE ${CORO_TARGETS}.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

export(
  EXPORT ${CORO_TARGETS}
  FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/${CORO_TARGETS}.cmake"
  NAMESPACE ${PROJECT_NAME}::)

include(CMakePackageConfigHelpers)

string(TOLOWER ${PROJECT_NAME} project_lower_)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/${project_lower_}-config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${project_lower_}-config-version.cmake"
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY AnyNewerVersion)

install(
  FILES
  "${CMAKE_CURRENT_BINARY_DIR}/${project_lower_}-config.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/${project_lower_}-config-version.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

unset(project_lower_)
