#!/usr/bin/env bash

make clean
make || exit
rm -f data.txt
rm -f ./*.png

for i in {1..16}; do
  printf '%s ' "$i" >>data.txt
  ./safe_list.out 20000000 "$i" 2>>data.txt || exit
  printf ' ' >>data.txt
  ./sp_safe_list.out 20000000 "$i" 2>>data.txt || exit
  printf '\n' >>data.txt
done

gnuplot <gnuplot_locks.txt || exit
echo "done"
