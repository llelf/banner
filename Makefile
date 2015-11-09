
FILES = banner.c re.c addr.c watcher.c
CFLAGS = -Wall -Wno-unused-function

banner: ${FILES} banner.h
	gcc ${CFLAGS} -o $@ ${FILES} -lpcre

