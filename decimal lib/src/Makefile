CC = gcc
CC_FLAGS = -Wall -Werror -Wextra -std=c11
FILE = s21_decimal.c
OS := $(shell uname -s)

ifeq ($(OS), Darwin)
	LDFLAGS = -fprofile-arcs -ftest-coverage -lcheck
else
	LDFLAGS = -fprofile-arcs -ftest-coverage -lcheck -lm -pthread -lsubunit
endif

all: s21_decimal.a test 

s21_decimal.a:
	$(CC) $(CC_FLAGS) -c $(FILE)
	ar r s21_decimal.a *.o
	rm *.o
	ranlib s21_decimal.a

test: s21_decimal.a
	$(CC) $(CC_FLAGS) tests_decimal.c s21_decimal.a $(LDFLAGS)
	./a.out
	make clean


gcov: 
	$(CC) $(CC_FLAGS) tests_decimal.c $(FILE) $(LDFLAGS)
	./a.out

gcov_report: gcov
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage_report
	open coverage_report/index.html
	make clean

style:
	clang-format -i --style=Google *.c *.h

clean: 
	rm -rf *.o *.gcno *.out  *.gcda *.info *.a

clean_report:
	rm -rf coverage_report/


rebuild: clean all
