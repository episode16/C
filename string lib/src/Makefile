CC = gcc
CC_FLAGS = -Wall -Werror -Wextra -std=c11
FILE = s21_sprintf.c s21_strerror.c s21_string.c
LDFLAGS = -fprofile-arcs -ftest-coverage -lcheck -lm -pthread

all: s21_string.a test 

s21_string.a:
	$(CC) $(CC_FLAGS) -c $(FILE)
	ar r s21_string.a *.o
	rm *.o
	ranlib s21_string.a

test: s21_string.a
	$(CC) $(CC_FLAGS) test.c s21_string.a $(LDFLAGS)
	./a.out
	make clean

gcov: 
	$(CC) $(CC_FLAGS) test.c $(FILE) $(LDFLAGS)
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
