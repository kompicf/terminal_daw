all: main.c src/*
	gcc main.c src/* -o e -lncurses -lportaudio -lm -Iinclude -Wall -Wextra -Wpedantic && ./e
debug: main.c src/*
	gcc main.c src/* -o e -g -lncurses -lportaudio -lm -Iinclude -Wall -Wextra -Wpedantic && gdb ./e
