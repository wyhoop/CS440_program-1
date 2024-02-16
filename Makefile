CFLAGS = -Wall -g

all: server

.PHONY: all clean

server: server.c
	${CC} ${CFLAGS} -o $@ server.c

clean:
	@rm -f server
