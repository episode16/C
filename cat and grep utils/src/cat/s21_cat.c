#include <getopt.h>
#include <stdio.h>
#include <string.h>

#include "cat.h"

int main(int argc, char *argv[]) {
  int index = 0;
  options flags = {0, 0, 0, 0, 0, 0, 0, 0};
  index = read_flags(argc, argv, &flags);
  if (index == 1) {
    printf("error: invalid option\n");
    return 1;
  } else {
    while (index < argc) {
      char *path = argv[index];
      FILE *file;
      file = fopen(path, "r");
      if (file == NULL) {
        printf("Error opening file: %s\n", path);
        return 1;
      }
      if (flags.s == 1) {
        file_reader_with_s(file);
      }
      if (flags.n == 1) {
        file_reader_with_n(file);
      }
      if (flags.b == 1) {
        file_reader_with_b(file);
      }
      if (flags.T == 1) {
        file_reader_with_T(file);
      }
      if (flags.E == 1) {
        file_reader_with_E(file);
      }
      if (flags.v == 1) {
        file_reader_with_v(file);
      }
      if (flags.e == 1) {
        file_reader_with_e(file);
      }
      if (flags.t == 1) {
        file_reader_with_t(file);
      }
      fclose(file);
      index++;
    }
  }
  return 0;
}

int read_flags(int argc, char *argv[], options *flags) {
  struct option longopts[] = {
      {"number-nonblank", 0, NULL, 'b'},
      {"number", 0, NULL, 'n'},
      {"squeeze-blank", 0, NULL, 's'},
      {NULL, 0, NULL, 0}  //окончание массива
  };
  int cur_flag = getopt_long(argc, argv, "benstvET", longopts, NULL);
  for (; cur_flag != -1;
       cur_flag = getopt_long(argc, argv, "benstvET", longopts, NULL)) {
    switch (cur_flag) {
      case 'b':
        flags->b = 1;
        break;
      case 'e':
        flags->e = 1;
        break;
      case 'E':
        flags->E = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 't':
        flags->t = 1;
        break;
      case 'T':
        flags->T = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      default:
        return 1;
    }
  }
  return optind;
}

void file_reader_with_s(FILE *file) {
  int prev = '\n';
  int i = 0;
  while ((i = fgetc(file)) != EOF) {
    if (prev == '\n' && i == '\n') {
      while ((i = fgetc(file)) == '\n') {
      }
      putchar('\n');
      if (i == EOF) break;
    }
    putchar(i);
    prev = i;
  }
}

void file_reader_with_b(FILE *file) {
  int prev = '\n';
  int i = 0;
  int line_number = 1;
  while ((i = fgetc(file)) != EOF) {
    if (prev == '\n' && i != '\n') {
      printf("%6d\t", line_number);
      line_number++;
    }
    putchar(i);
    prev = i;
  }
}

void file_reader_with_n(FILE *file) {
  int prev = '\n';
  int i = 0;
  int line_number = 1;
  while ((i = fgetc(file)) != EOF) {
    if ((prev == '\n' && i != '\n') || (prev == '\n' && i == '\n')) {
      printf("%6d\t", line_number);
      line_number++;
    }
    putchar(i);
    prev = i;
  }
}

void file_reader_with_T(FILE *file) {
  int i = 0;
  while ((i = fgetc(file)) != EOF) {
    if (i == '\t') {
      printf("^I");
    } else {
      putchar(i);
    }
  }
}

void file_reader_with_E(FILE *file) {
  int i = 0;
  while ((i = fgetc(file)) != EOF) {
    if (i == '\n') {
      printf("$\n");
    } else {
      putchar(i);
    }
  }
}

void file_reader_with_v(FILE *file) {
  int i = 0;
  while ((i = fgetc(file)) != EOF) {
    if ((i >= 0 && i <= 31) && i != '\t' && i != '\n') {
      printf("^%c", i + 64);
    } else if (i == 127) {
      printf("^?");
    } else if ((i >= 128 && i < 128 + 32) && i != '\t' && i != '\n') {
      printf("M-^%c", i - 64);
    } else {
      putchar(i);
    }
  }
}

void file_reader_with_e(FILE *file) {
  int i = 0;
  while ((i = fgetc(file)) != EOF) {
    if ((i >= 0 && i <= 31) && i != '\t' && i != '\n') {
      printf("^%c", i + 64);
    } else if (i == 127) {
      printf("^?");
    } else if ((i >= 128 && i < 128 + 32) && i != '\t' && i != '\n') {
      printf("M-^%c", i - 64);
    } else if (i == '\n') {
      printf("$\n");
    } else {
      putchar(i);
    }
  }
}

void file_reader_with_t(FILE *file) {
  int i;
  while ((i = fgetc(file)) != EOF) {
    if ((i >= 0 && i <= 31) && i != '\t' && i != '\n') {
      printf("^%c", i + 64);
    } else if (i == 127) {
      printf("^?");
    } else if ((i >= 128 && i < 128 + 32) && i != '\t' && i != '\n') {
      printf("M-^%c", i - 64);
    } else if (i == '\t') {
      printf("^I");
    } else {
      putchar(i);
    }
  }
}
