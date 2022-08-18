#!/usr/bin/env bash

make || exit
rm -f data.txt
rm -f ./*.png

for i in {1..8}; do
  printf '%s ' "$i" >>data.txt
  ./false_sharing.out "$i" 2>>data.txt || exit
done

sed 's/PLACE/same/g' gnuplot.txt | gnuplot

rm -f data.txt

for i in {1..8}; do
  printf '%s ' "$i" >>data.txt
  ./no_false_sharing.out "$i" 2>>data.txt || exit
done

sed 's/PLACE/diff/g' gnuplot.txt | gnuplot
echo "done"
