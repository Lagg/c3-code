CFLAGS = -ansi -pedantic -Wall -Wextra -Wno-long-long -g
BINARIES = alignments unions sockets journal

COMPILE_LINE = $(CC) -c $(CFLAGS) -o $@ $<
LINK_LINE = $(CC) $(LDFLAGS) -o $@ $^

all: $(BINARIES)

alignments: alignments.o
	$(LINK_LINE)

alignments.o: alignments.c
	$(COMPILE_LINE)

unions: unions.o
	$(LINK_LINE)

unions.o: unions.c
	$(COMPILE_LINE)

sockets: socket.o
	$(LINK_LINE)

socket.o: socket.c
	$(COMPILE_LINE) -D_POSIX_SOURCE

journal: journal.o
	$(LINK_LINE)

journal.o: journal.c
	$(COMPILE_LINE)

.PHONY: clean
clean:
	rm -f *.o $(BINARIES)
