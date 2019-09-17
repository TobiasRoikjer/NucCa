# cmake_minimum_required(VERSION <specify CMake version here>)
project(cproj C)

SET(CMAKE_C_FLAGS "-std=c11 ${CMAKE_C_FLAGS}")
set(CMAKE_C_STANDARD 11)

add_executable(cproj main.c ds.h)
add_executable(nuc_fletch nuc_fletch.c utils.h ds.h)
add_executable(uniq_line uniq_line.c utils.h)
add_executable(assign_nuc assign_nuc.c utils.h file.c file.h)
add_executable(test test/test.c file.h file.c)
