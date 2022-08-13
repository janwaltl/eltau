cmake_minimum_required(VERSION 3.15..3.18)

# Project global options
project(eltau VERSION 0.1 LANGUAGES CXX)

option(ELTAU_ENABLE_COVERAGE "Enable code coverage testing support" OFF)
option(ELTAU_JUNIT_TEST_OUTPUT "Generate test reports in JUNIT(XML) format" OFF)
option(ELTAU_BUILD_EXAMPLES "Build examples" OFF)
option(ELTAU_BUILD_TEST "Build tests" OFF)
option(ELTAU_SANITIZED "Sanitized build (addr,leak,UB)" OFF)

# Set reasonable build options for Elphi project targets
function(config_default_target_flags target)
  target_compile_options(${target} PRIVATE -std=c++20 -Wall -Wextra -Werror -pedantic -fpic -fno-omit-frame-pointer)
  target_compile_options(${target} PRIVATE -D_POSIX_C_SOURCE=200809)

  if(ELTAU_SANITIZED)
    target_compile_options(${target} PRIVATE -fsanitize=address,leak,undefined)
    target_link_options(${target} PRIVATE -fsanitize=address,leak,undefined)
  endif(ELTAU_SANITIZED)

  if(CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_options(${target} PRIVATE -Og -g3)
  else()
    target_compile_options(${target} PRIVATE -O2 -g3)
    target_link_options(${target} PRIVATE -flto)
  endif()

  if(ELTAU_ENABLE_COVERAGE)
    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
      message(WARNING "Running coverage for non-debug build of \'${target}\'")
    endif(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${target} PRIVATE --coverage)
    target_link_options(${target} PRIVATE --coverage)
  endif(ELTAU_ENABLE_COVERAGE)

endfunction()

add_subdirectory(libs/catch2)
add_subdirectory(libs/fmt)
add_subdirectory(libs/ftxui)

if(ELTAU_ENABLE_COVERAGE)
  include(cmake/CodeCoverage.cmake)
  set(GCOVR_ADDITIONAL_ARGS --xml "coverage.xml")
  setup_target_for_coverage_gcovr_html(
    NAME coverage
    EXECUTABLE ctest -C
    DEPENDENCIES elphi::elphi elphi::elphi_tests
    EXCLUDE "eltau/test/*"
    EXCLUDE "eltau/examples/*"
    EXCLUDE "libs*"
  )
endif(ELTAU_ENABLE_COVERAGE)

if(ELTAU_BUILD_TEST)
  enable_testing()
endif(ELTAU_BUILD_TEST)

# Eltau library

add_library(eltau STATIC)
add_library(eltau::eltau ALIAS eltau)

config_default_target_flags(eltau)

target_include_directories(
  eltau
  PUBLIC
  include
)

target_link_libraries(eltau PRIVATE fmt::fmt)

target_sources(
  eltau
  PUBLIC
  include/eltau/element.hpp
  include/eltau/exception.hpp
  include/eltau/screen.hpp
  include/eltau/terminal.hpp
  PRIVATE
  src/element.cpp
  src/exception.cpp
  src/screen.cpp
  src/terminal.cpp
)

if(ELTAU_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif(ELTAU_BUILD_EXAMPLES)
if(ELTAU_BUILD_TEST)
  add_subdirectory(test)
endif(ELTAU_BUILD_TEST)
