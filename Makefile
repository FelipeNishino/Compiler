build:
	gcc main.c -std=c99 -Wall -o main

run: build
	./main