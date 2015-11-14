
FILES = banner.c match.c addr.c watcher.c misc.c act.c
CFLAGS = -std=c99 -Wall -Wno-unused-function

banner: ${FILES} banner.h
	gcc ${CFLAGS} -o $@ ${FILES} -lpcre

