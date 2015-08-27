
FILES = banner.c re.c addr.c
CFLAGS = -Wall -Wno-unused-function

banner: ${FILES}
	gcc ${CFLAGS} -o $@ $^ -lpcre

