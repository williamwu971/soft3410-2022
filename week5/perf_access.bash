#!/usr/bin/env bash

gcc access.c || exit

#sudo perf stat -e cache-misses,cache-references,L1-dcache-loads,L1-dcache-load-misses ./a.out rand 64 1000000
#sudo perf stat -e cache-misses,cache-references,L1-dcache-loads,L1-dcache-load-misses ./a.out seq 64 1000000

./a.out rand 64 1000000
./a.out seq 64 1000000
