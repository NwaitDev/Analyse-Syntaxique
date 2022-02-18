cmake_minimum_required(VERSION 2.8)

project(BISON_TEST C)

set(CMAKE_C_FLAGS "-Wall -std=c99 -O2 -g")

find_package(BISON)
find_package(FLEX)

bison_target(parser
  parser.y
  ${CMAKE_CURRENT_BINARY_DIR}/parser.c
  COMPILE_FLAGS "-v"
)

flex_target(lexer
  lexer.l
  ${CMAKE_CURRENT_BINARY_DIR}/lexer.c
  DEFINES_FILE "${CMAKE_CURRENT_BINARY_DIR}/lexer.h"
)

add_flex_bison_dependency(lexer parser)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(bison-test
  main.c
  ${BISON_parser_OUTPUTS}
  ${FLEX_lexer_OUTPUTS}
)

target_compile_definitions(bison-test
  PRIVATE
    _POSIX_C_SOURCE=200809L
)
