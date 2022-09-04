CC=gcc
TARGETS=linkedlist_slow.out linkedlist.out
CFLAGS=-w

all: $(TARGETS)

push:
	git commit -am "-AUTO- `date`" || true
	git push || true

linkedlist_slow.out: linkedlist_slow.c
	$(CC) $(CFLAGS) $^ -o $@

linkedlist.out: linkedlist.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(TARGETS)

