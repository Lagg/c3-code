CFLAGS = -ansi -pedantic -Wall -Wextra -Wno-long-long -g

all: alignments unions sockets journal

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

journal: journal.o
	$(CC) $(LDFLAGS) -o $@ $^

journal.o: journal.c
	$(CC) -c $(CFLAGS) -D_POSIX_SOURCE $(LDFLAGS) -o $@ $<

clean: .PHONY
	rm -f *.o alignments unions sockets journal

.PHONY:
