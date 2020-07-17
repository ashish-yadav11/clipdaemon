CC=gcc
CFLAGS=-Wall -Werror -O3

clipdaemon: clipdaemon.c
	${CC} -o clipdaemon ${CFLAGS} `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` clipdaemon.c

clean:
	rm -f clipdaemon
