cmake_minimum_required(VERSION 3.29)
project(niceProjectName C)

set(CMAKE_C_STANDARD 11)

add_executable(niceProjectName Src/main.c
        Include/main.h)
