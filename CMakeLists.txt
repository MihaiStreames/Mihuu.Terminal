cmake_minimum_required(VERSION 3.26)
project(TerminalMihuu C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

add_executable(TerminalMihuu
        main.c
        common.h

        handlers/input_parser.c
        handlers/input_handler.c
        handlers/signal_handler.c

        builtins/cd.c
        builtins/builtins.h
)