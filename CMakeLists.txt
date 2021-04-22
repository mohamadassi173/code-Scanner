cmake_minimum_required(VERSION 3.16)
project(code_Scanner)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(code_Scanner
        C.l
        main.cpp
        reserved.txt
        scanner.cpp
        scanner.h
        symtab.cpp
        symtab.h
        token.cpp
        token.h
        tokenstrings.h
        tokentype.h)
