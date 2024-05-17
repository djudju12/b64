build:
	gcc -o b64 b64.c -Wall -Wextra -ggdb

run: build
	./b64

debug: build
	gf2 b64