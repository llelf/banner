
FILES = banner.c re.c

banner: ${FILES}
	gcc -o $@ $^ -lpcre

