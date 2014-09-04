CFLAGS = -ansi -pedantic -Wall -Wextra

all: alignments unions

alignments: alignments.o
	$(CC) $(LDFLAGS) -o $@ $^

alignments.o: alignments.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) -o $@ $<

unions: unions.o
	$(CC) $(LDFLAGS) -o $@ $^

unions.o: unions.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) -o $@ $<

clean: .PHONY
	rm -f *.o alignments unions

.PHONY:
