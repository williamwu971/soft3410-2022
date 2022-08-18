#!/usr/bin/env bash

rm -f data.txt

for i in {1..4}; do
  printf '%s ' "$i" >>data.txt
  ./false_sharing.out "$i" 2>>data.txt || exit
done

gnuplot <gnuplot.txt
