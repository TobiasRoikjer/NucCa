# cmake_minimum_required(VERSION <specify CMake version here>)
project(cproj C)

set(CMAKE_C_STANDARD 11)

add_executable(cproj main.c ds.h)
add_executable(nuc_fletch nuc_fletch.c utils.h ds.h)
add_executable(uniq_line uniq_line.c utils.h)