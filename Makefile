CFLAGS = -Wall -g

all: server client

.PHONY: all clean

server: server.c
	${CC} ${CFLAGS} -o $@ server.c

client: client.c
	${CC} ${CFLAGS} -o $@ client.c

clean:
	@rm -f server client
