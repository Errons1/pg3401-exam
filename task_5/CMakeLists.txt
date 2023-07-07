cmake_minimum_required(VERSION 3.25)
project(task_5 C)

set(CMAKE_C_STANDARD 99)

add_executable(task_5 src/main.c src/client.c src/server.c src/protocol.h src/eksamen_v23_oppgave5_exec.c src/eksamen_v23_oppgave5_exec.h src/protocol.c lib/error/error.c lib/input/input.c src/main.h)
