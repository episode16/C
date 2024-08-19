SHELL := /bin/bash
CC = gcc
CC_FLAGS = -Wall -Werror -Wextra -std=c11
BUILD_NAME = s21_grep

all: s21_grep test

s21_grep: s21_grep.o
	${CC} s21_grep.o -o ${BUILD_NAME}

s21_grep.o: s21_grep.c
	${CC} -c s21_grep.c ${CC_FLAGS} -o s21_grep.o

test:
	sh test.sh > test_grep.txt

clean:
	rm -rf ./*.o    rm -rf ./${BUILD_NAME}

rebuild: clean s21_grep