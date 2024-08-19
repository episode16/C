#ifndef GREP_H
#define GREP_H
#define _GNU_SOURCE
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int e;            // done
  int i;            // done
  int v;            // done
  int c;            // done
  int l;            // done
  int n;            // done
  int h;            // done
  int s;            // done
  int f;            // done
  int o;            // done
  int len_pattern;  // C:
  char pattern[9999];
} options;

void print_line(char *line, int n);
int read_flags(int argc, char *argv[], options *flags);
void file_reader(FILE *file, options flags, char *path);
void pattern_add(options *flags, char *pattern);
void opt_flag_f(options *flags, char *path);
void opt_flag_o(regex_t *re, char *line, options flags, char *path);

#endif