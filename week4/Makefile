TARGET=matrix.out false_sharing.out no_false_sharing.out safe_list.out sp_safe_list.out

all: $(TARGET)

matrix.out: matrix.c
	gcc $^ -o $@ -fopenmp

false_sharing.out: false_sharing.c
	gcc $^ -o $@ -DFALSE

no_false_sharing.out: false_sharing.c
	gcc $^ -o $@

safe_list.out: safe_list.c
	gcc $^ -o $@

sp_safe_list.out: safe_list.c
	gcc $^ -o $@ -DSP

clean:
	rm -f $(TARGET) *.png *.out data.txt

push:
	git commit -am "-AUTO- `date`" || true
	git push || true

