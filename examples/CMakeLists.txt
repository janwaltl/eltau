add_executable(example)
add_executable(eltau::example ALIAS example)

config_default_target_flags(example)

target_link_libraries(example PRIVATE fmt::fmt eltau::eltau)

target_sources(
  example
  PRIVATE
  basic.cpp
)
