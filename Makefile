CFLAGS = -ansi -pedantic -Wall -Wextra

all: alignments

alignments: alignments.o
	$(CC) $(LDFLAGS) -o $@ $^

alignments.o: alignments.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:dummy
	rm -f *.o

dummy:
