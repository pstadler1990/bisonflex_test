cmake_minimum_required(VERSION 3.13)
project(evoscript)
set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c evoscript.c grammar.tab.c lex.yy.c)

add_executable(evoscript ${SOURCE_FILES})