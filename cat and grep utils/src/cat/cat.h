#ifndef S21_CAT_H
#define S21_CAT_H
#include <getopt.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  int b;  // done
  int e;  // done
  int n;  // done
  int s;  // done
  int t;  // done
  int v;  // done
  int E;  // done
  int T;  // done
} options;

int read_flags(int argc, char *argv[], options *flags);
void file_reader_with_b(FILE *file);
void file_reader_with_e(FILE *file);
void file_reader_with_s(FILE *file);
void file_reader_with_n(FILE *file);
void file_reader_with_T(FILE *file);
void file_reader_with_t(FILE *file);
void file_reader_with_E(FILE *file);
void file_reader_with_v(FILE *file);

#endif