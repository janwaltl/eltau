cmake_minimum_required(VERSION 3.15..3.18)

# Project global options
project(eltau VERSION 0.1 LANGUAGES CXX)

option(ELTAU_ENABLE_COVERAGE "Enable code coverage testing support" OFF)
option(ELTAU_JUNIT_TEST_OUTPUT "Generate test reports in JUNIT(XML) format" OFF)
option(ELTAU_BUILD_EXAMPLES "Build examples" ON)
option(ELTAU_BUILD_TEST "Build tests" ON)
option(ELTAU_ASAN "Sanitized build (addr,leak,UB)" OFF)
option(ELTAU_MSAN "Sanitized build (memory)" OFF)
option(ELTAU_TSAN "Sanitized build (thread)" OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

include(cmake/project_options.cmake)

add_subdirectory(libs/catch2)
add_subdirectory(libs/fmt)

if(ELTAU_ENABLE_COVERAGE)
  include(cmake/CodeCoverage.cmake)
  set(GCOVR_ADDITIONAL_ARGS --xml "coverage.xml")
  setup_target_for_coverage_gcovr_html(
    NAME coverage
    EXECUTABLE ctest -C
    DEPENDENCIES eltau::eltau eltau::tests
    EXCLUDE "test/*"
    EXCLUDE "examples/*"
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
  include/eltau/coords.hpp
  include/eltau/element.hpp
  include/eltau/exception.hpp
  include/eltau/screen.hpp
  include/eltau/terminal.hpp
  PRIVATE
  src/coords.cpp
  src/element.cpp
  src/text.cpp
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
