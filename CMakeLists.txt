cmake_minimum_required(VERSION 3.16)
project(soft3410_2022 C)

set(CMAKE_C_STANDARD 99)

add_executable(soft3410_2022 main.c week2/linkedlist_map.c main.h week2/linkedlist_map.h week2/stack.c week2/stack.h
        week2/sorting.c week2/sorting.h week2/flip.c
        week3/resize_nn.c week3/perf_qn.c week3/unrolling_qn.c
        hw1/dynamic_array_map.c hw1/dynamic_array_map.h week4/welcome.c week4/parallel_sum.c week4/false_sharing.c
        week4/matrix.c nbody/nbody.h nbody/nbody.c week4/safe_list.c student/quanhao_false.c student/quanhao_non_false.c)
