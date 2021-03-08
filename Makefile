PREFIX = /usr/local

CC = gcc
CFLAGS = -O3 -Wall -Wextra

clipdaemon: clipdaemon.c
	${CC} -o $@ ${CFLAGS} `pkg-config --cflags gtk+-3.0` clipdaemon.c `pkg-config --libs gtk+-3.0`

clean:
	rm -f clipdaemon

install: clipdaemon
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f clipdaemon ${DESTDIR}${PREFIX}/bin/clipdaemon
	chmod 755 ${DESTDIR}${PREFIX}/bin/clipdaemon

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/clipdaemon

.PHONY: clean install uninstall
