#!/usr/bin/env bash

gcc matrix.c || exit
rm -f data.txt
rm -f ./*.png

for i in {6..10}; do
  printf '%s ' "$((2 ** i))" >>data.txt
  ./a.out $((2 ** i)) 0 2>>data.txt || exit
  printf ' ' >>data.txt
  ./a.out $((2 ** i)) 1 2>>data.txt || exit
  printf '\n' >>data.txt
done

gnuplot <gnuplot_false_sharing.txt || exit
echo "done"
