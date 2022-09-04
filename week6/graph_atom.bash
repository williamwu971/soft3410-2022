#!/usr/bin/env bash

gcc atom.c
rm -f data.txt
rm -f ./*.png

for i in {1..8}; do
  printf '%s ' "$i" >>data.txt
  ./a.out "$i" 2>>data.txt || exit
  printf '\n' >>data.txt
done

gnuplot <gnuplot_atom.txt || exit
echo "done"
