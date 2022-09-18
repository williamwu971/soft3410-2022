cmake_minimum_required(VERSION 3.16)
project(soft3410_2022 C)

set(CMAKE_C_STANDARD 99)

add_executable(soft3410_2022 main.c week2/linkedlist_map.c main.h week2/linkedlist_map.h week2/stack.c week2/stack.h
        week2/sorting.c week2/sorting.h week2/flip.c
        week3/resize_nn.c week3/perf_qn.c week3/unrolling_qn.c
        hw1/dynamic_array_map.c hw1/dynamic_array_map.h week4/welcome.c week4/parallel_sum.c week4/false_sharing.c
        week4/matrix.c nbody/nbody.h nbody/nbody.c week4/safe_list.c student/quanhao_false.c student/quanhao_non_false.c week5/access.c week5/mqsort.c week5/processes.c week5/ham.c student/kang_false_sharing.c hw2/matrix.c week6/sum.c week6/phil.c week6/atom.c week6/spin.c week6/spin.h week6/linkedlist.c week6/linkedlist_slow.c week6/sem_vs_mutex.c week7/ref.c week7/ref.h week7/atom_stack.c week7/atom_stack.h week7/atom_main.c week7/mutex_stack.c week8/dynamic_array.c week8/busy.c week8/busy.h week8/ftp.c week8/thread_pool.c)
