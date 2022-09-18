#!/usr/bin/env bash

gcc dynamic_array.c -O0
rm -f data.txt
rm -f ./*.png

for i in {10..90..10}; do
  printf '%s ' "$i" >>data.txt
  ./a.out 0 "$i" 2>>data.txt || exit
  ./a.out 1 "$i" 2>>data.txt || exit
  printf '\n' >>data.txt
done

gnuplot <gnuplot_locks.txt || exit
echo "done"
