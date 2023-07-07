cmake_minimum_required(VERSION 3.25)
project(task_3 C)

set(CMAKE_C_STANDARD 99)

add_executable(task_3 src/main.c src/LinkedList/ll_flights.c src/LinkedList/ll_flights.h src/program.c src/program.h lib/input/input.c src/program.c)
