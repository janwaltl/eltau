add_executable(tests)
add_executable(eltau::tests ALIAS tests)

target_link_libraries(tests PRIVATE eltau::eltau)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain fmt::fmt)


config_default_target_flags(tests)

include(CTest)
include(Catch)

make_directory("${CMAKE_BINARY_DIR}/reports")
if(ELTAU_JUNIT_TEST_OUTPUT)
  catch_discover_tests(
    tests
    OUTPUT_DIR "${CMAKE_BINARY_DIR}/reports"
    OUTPUT_SUFFIX ".xml"
    REPORTER junit
  )
else()
  catch_discover_tests(
    tests
    EXTRA_ARGS --colour-mode ansi
    OUTPUT_DIR "${CMAKE_BINARY_DIR}/reports"
    OUTPUT_SUFFIX ".txt"
  )
endif(ELTAU_JUNIT_TEST_OUTPUT)

target_sources(
  tests
  PRIVATE
  test_screen.cpp
  test_exception.cpp
)
