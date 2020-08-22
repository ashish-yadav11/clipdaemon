#PREFIX = /usr/local
PREFIX = ${HOME}/.local

CC = gcc
CFLAGS = -O3 -Wall -Werror

clipdaemon: clipdaemon.c
	${CC} -o $@ ${CFLAGS} `pkg-config --cflags gtk+-3.0` $< `pkg-config --libs gtk+-3.0`

clean:
	rm -f clipdaemon

install: clipdaemon
	mkdir -p ${DESTDIR}${PREFIX}/bin
	install -m 0755 clipdaemon ${DESTDIR}${PREFIX}/bin/clipdaemon

.PHONY: clean install
