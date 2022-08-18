TARGET=matrix.out false_sharing.out

all: $(TARGET)

matrix.out: matrix.c
	gcc matrix.c -o matrix -fopenmp

false_sharing.out: false_sharing.c
	gcc $^ -o $@

clean:
	rm -f $(TARGET)