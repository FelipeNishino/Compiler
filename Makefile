build:
	gcc main.c iomanager.c -std=c99 -Wall -o ./bin/main

run: build
	./bin/main

debugI: build
	./bin/main input.txt