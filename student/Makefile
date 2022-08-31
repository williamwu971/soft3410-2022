TARGET=false_sharing.out no_false_sharing.out

all: $(TARGET)
CC=gcc

false_sharing.out: kang_false_sharing.c
	${CC} $^ -o $@ -pthread -DCACHE_LINE_ALIGN

no_false_sharing.out: kang_false_sharing.c
	${CC} $^ -o $@ -pthread

clean:
	rm -f $(TARGET) *.png *.out data.txt

push:
	git commit -am "-AUTO- `date`" || true
	git push || true

download:
	scp 'xiaoxiang@labos2.cs.usyd.edu.au:/mnt/sdb/xiaoxiang/soft3410-2022/week4/*.png' .

