SHELL := /bin/bash
CC = gcc
CC_FLAGS = -Wall -Werror -Wextra -std=c11
BUILD_NAME = s21_cat

all: s21_cat test

s21_cat: s21_cat.o
	${CC} s21_cat.o -o ${BUILD_NAME}

s21_cat.o: s21_cat.c
	${CC} -c s21_cat.c ${CC_FLAGS} -o s21_cat.o

test:
	-diff <(./s21_cat -e test1.txt test2.txt) <(cat -e test1.txt test2.txt)
	-diff <(./s21_cat -v test1.txt test2.txt) <(cat -v test1.txt test2.txt)
	-diff <(./s21_cat -s test1.txt test2.txt) <(cat -s test1.txt test2.txt)
	-diff <(./s21_cat -n test1.txt test2.txt) <(cat -n test1.txt test2.txt)
	-diff <(./s21_cat -t test1.txt test2.txt) <(cat -t test1.txt test2.txt)
	-diff <(./s21_cat -b test1.txt test2.txt) <(cat -b test1.txt test2.txt)

clean:
	rm -rf ./*.o    rm -rf ./${BUILD_NAME}

rebuild: clean s21_cat