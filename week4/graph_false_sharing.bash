#!/usr/bin/env bash

make clean
make || exit
rm -f data.txt
rm -f ./*.png

for i in {1..8}; do
  printf '%s ' "$i" >>data.txt
  taskset -c "$(lscpu | grep On-line | awk '{print $4}')":2 ./false_sharing.out "$i" 2>>data.txt || exit
  printf ' ' >>data.txt
  taskset -c "$(lscpu | grep On-line | awk '{print $4}')":2 ./no_false_sharing.out "$i" 2>>data.txt || exit
  printf '\n' >>data.txt
done

gnuplot <gnuplot.txt || exit
echo "done"
