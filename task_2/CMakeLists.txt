cmake_minimum_required(VERSION 3.25)
project(task_2 C)

set(CMAKE_C_STANDARD 99)


add_executable(task_2 src/main.c lib/eksamen_v23_oppgave2_abun.c lib/eksamen_v23_oppgave2_cube.c lib/eksamen_v23_oppgave2_def.c lib/eksamen_v23_oppgave2_fib.c lib/eksamen_v23_oppgave2_kvad.c lib/eksamen_v23_oppgave2_odd.c lib/eksamen_v23_oppgave2_perf.c lib/eksamen_v23_oppgave2_prim.c lib/eksamen_v23_oppgave2_kvad.h lib/eksamen_v23_oppgave2_fib.h lib/eksamen_v23_oppgave2_def.h lib/eksamen_v23_oppgave2_cube.h lib/eksamen_v23_oppgave2_abun.h lib/eksamen_v23_oppgave2_odd.h lib/eksamen_v23_oppgave2_perf.h lib/eksamen_v23_oppgave2_prim.h src/analyzeNumbersFromFile.c src/analyzeNumbersFromFile.h)
target_link_libraries(task_2 m)
