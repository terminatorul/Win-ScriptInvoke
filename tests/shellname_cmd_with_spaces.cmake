#!"C:\Program Files\CMake\bin\cmake.exe" "-DSTRING_VARIABLE=Configuration 2" -P

cmake_minimum_required(VERSION 3.10)

if(NOT DEFINED STRING_VARIABLE OR NOT STRING_VARIABLE STREQUAL "Configuration 2")
    message(FATAL_ERROR "No variable received on the command line")
endif()
