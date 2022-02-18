cmake_minimum_required(VERSION 2.8)

project(FLEX_TEST C)

set(CMAKE_C_FLAGS "-Wall -std=c99 -O2 -g")

find_package(FLEX)

flex_target(lexer
  lexer.l
  ${CMAKE_CURRENT_BINARY_DIR}/lexer.c
  DEFINES_FILE "${CMAKE_CURRENT_BINARY_DIR}/lexer.h"
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(flex-test
  ${FLEX_lexer_OUTPUTS}
  main.c
)

target_compile_definitions(flex-test
  PRIVATE
    _POSIX_C_SOURCE=200809L
)
