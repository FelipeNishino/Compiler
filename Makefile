DIR := ${CURDIR}

build:
	gcc main.c iomanager.c dfa.c token.c lexer.c -std=c99 -Wall -o ${DIR}/bin/main

run: build
	./bin/main

debugI: build
	./bin/main input.txt