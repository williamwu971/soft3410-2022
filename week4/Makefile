TARGET=matrix.out false_sharing.out no_false_sharing.out

all: $(TARGET)

matrix.out: matrix.c
	gcc $^ -o $@ -fopenmp

false_sharing.out: false_sharing.c
	gcc $^ -o $@ -DFALSE

no_false_sharing.out: false_sharing.c
	gcc $^ -o $@

clean:
	rm -f $(TARGET)

push:
	git commit -am "-AUTO- `date`" || true
	git push || true

