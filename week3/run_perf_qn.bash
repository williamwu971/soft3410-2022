#!/usr/bin/env bash

gcc perf_qn.c -ggdb3 -o program || exit
/home/blepers/linux-huge/tools/perf/perf record -F 100 -a -g -- taskset -c 2 ./program
/home/blepers/linux-huge/tools/perf/perf script >stackdata.perf
../../FlameGraph/stackcollapse-perf.pl stackdata.perf >stackdata.folded
../../FlameGraph/flamegraph.pl stackdata.folded >output.svg
