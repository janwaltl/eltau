# Set reasonable build options for Elphi project targets
function(config_default_target_flags target)
  target_compile_options(${target} PRIVATE -std=c++20 -Wall -Wextra -Werror -pedantic -fpic -fno-omit-frame-pointer)
  target_compile_options(${target} PRIVATE -D_POSIX_C_SOURCE=200809)

  if(ELTAU_ASAN)
    target_compile_options(${target} PRIVATE -fsanitize=address,leak,undefined)
    target_link_options(${target} PRIVATE -fsanitize=address,leak,undefined)
  endif(ELTAU_ASAN)

  if(ELTAU_MSAN)
    target_compile_options(${target} PRIVATE -fsanitize=memory)
    target_link_options(${target} PRIVATE -fsanitize=memory)
  endif(ELTAU_MSAN)

  if(ELTAU_TSAN)
    target_compile_options(${target} PRIVATE -fsanitize=thread)
    target_link_options(${target} PRIVATE -fsanitize=thread)
  endif(ELTAU_TSAN)

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
