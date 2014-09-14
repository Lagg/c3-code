CFLAGS = -ansi -pedantic -Wall -Wextra

all: alignments unions sockets

alignments: alignments.o
	$(CC) $(LDFLAGS) -o $@ $^

alignments.o: alignments.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) -o $@ $<

unions: unions.o
	$(CC) $(LDFLAGS) -o $@ $^

unions.o: unions.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) -o $@ $<

sockets: socket.o
	$(CC) $(LDFLAGS) -o $@ $^

socket.o: socket.c
	$(CC) -c $(CFLAGS) -D_POSIX_SOURCE $(LDFLAGS) -o $@ $<

clean: .PHONY
	rm -f *.o alignments unions sockets

.PHONY:
