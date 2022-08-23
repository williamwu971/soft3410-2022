#!/usr/bin/env bash

make clean
make || exit
rm -f data.txt
rm -f ./*.png

for i in {1..8}; do
  printf '%s ' "$i" >>data.txt
  ./false_sharing.out "$i" 2>>data.txt || exit
  printf ' ' >>data.txt
  ./no_false_sharing.out "$i" 2>>data.txt || exit
  printf '\n' >>data.txt
done

gnuplot <gnuplot_false_sharing.txt || exit
echo "done"
