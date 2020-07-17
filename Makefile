#PREFIX = /usr/local
PREFIX = ${HOME}/.local

CC=gcc
CFLAGS=-Wall -Werror -O3

clipdaemon: clipdaemon.c
	${CC} -o clipdaemon ${CFLAGS} `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` clipdaemon.c

clean:
	rm -f clipdaemon

install:
	mkdir -p ${DESTDIR}${PREFIX}/bin
	install -m 0755 clipdaemon ${DESTDIR}${PREFIX}/bin/clipdaemon

.PHONY: clean install
