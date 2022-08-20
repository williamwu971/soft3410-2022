TARGET=matrix.out false_sharing.out no_false_sharing.out safe_list.out sp_safe_list.out

all: $(TARGET)
CC=gcc

matrix.out: matrix.c
	${CC} $^ -o $@ -fopenmp -pthread

false_sharing.out: false_sharing.c
	${CC} $^ -o $@ -DFALSE -pthread

no_false_sharing.out: false_sharing.c
	${CC} $^ -o $@ -pthread

safe_list.out: safe_list.c
	${CC} $^ -o $@ -pthread

sp_safe_list.out: safe_list.c
	${CC} $^ -o $@ -DSP -pthread

clean:
	rm -f $(TARGET) *.png *.out data.txt

push:
	git commit -am "-AUTO- `date`" || true
	git push || true

download:
	scp 'xiaoxiang@labos2.cs.usyd.edu.au:/mnt/sdb/xiaoxiang/soft3410-2022/week4/*.png' .

