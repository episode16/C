#include "grep.h"

int main(int argc, char *argv[]) {
  int index = 0;
  options flags = {0};
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
        if (!flags.s)
          fprintf(stderr, "grep: %s: No such file or directory", path);
      } else {
        file_reader(file, flags, path);
      }
      fclose(file);
      index++;
    }
  }
  return 0;
}

int read_flags(int argc, char *argv[], options *flags) {
  int cur_flag = getopt(argc, argv, "e:ivclnhsf:o");
  for (; cur_flag != -1; cur_flag = getopt(argc, argv, "e:ivclnhsf:o")) {
    switch (cur_flag) {
      case 'e':
        flags->e = 1;
        pattern_add(flags, optarg);
        break;
      case 'i':
        flags->i = REG_ICASE;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'f':
        flags->f = 1;
        opt_flag_f(flags, optarg);
        break;
      case 'o':
        flags->o = 1;
        break;
      default:
        return 1;
    }
  }
  if ((flags->v == 1 || flags->c == 1 || flags->l == 1) && (flags->o == 1)) {
    flags->o = 0;
  }
  if (flags->len_pattern == 0) {
    pattern_add(flags, argv[optind]);
    optind++;
  }
  if (argc - optind == 1) {
    flags->h = 1;
  }
  return optind;
}

void file_reader(FILE *file, options flags, char *path) {
  regex_t re;
  int error = regcomp(&re, flags.pattern, REG_EXTENDED | flags.i);
  if (error)
    exit(1);
  else {
    char *line = NULL;
    size_t memlen = 0;
    int read = 0;
    int line_count = 1;
    int counter = 0;
    while ((read = getline(&line, &memlen, file)) != -1) {
      int result = regexec(&re, line, 0, NULL, 0);
      if ((!flags.v && result == 0) || (flags.v && result != 0)) {
        counter++;
        if (!flags.c && !flags.l) {
          if (!flags.h && !flags.o) printf("%s:", path);
          if (flags.n) printf("%d:", line_count);
          if (flags.o) {
            opt_flag_o(&re, line, flags, path);
          } else {
            print_line(line, read);
          }
        }
      }
      line_count++;
    }
    free(line);
    if (flags.c && !flags.l) {
      if (!flags.h) printf("%s:", path);
      printf("%d\n", counter);
    } else if (flags.c && flags.l) {
      if (!flags.h) {
        printf("%s:", path);
      } else {
        printf("%d\n", 1);
        printf("%s\n", path);
      }
    } else if (flags.l && counter > 0)
      printf("%s\n", path);
  }
  regfree(&re);
}

// вывод строки
void print_line(char *line, int n) {
  for (int i = 0; i < n; i++) {
    if (line[i] != '\n') putchar(line[i]);
  }
  puts("");
}

void pattern_add(options *flags, char *pattern) {
  if (flags->len_pattern != 0) {
    strcat(flags->pattern + flags->len_pattern, "|");
    flags->len_pattern++;
  }
  flags->len_pattern +=
      sprintf(flags->pattern + flags->len_pattern, "(%s)", pattern);
}

void opt_flag_f(options *flags, char *path) {
  FILE *file;
  file = fopen(path, "r");
  if (file == NULL) {
    if (!flags->s) fprintf(stderr, "grep: %s: No such file or directory", path);
  } else {
    char *line = NULL;
    size_t memlen = 0;
    int read = getline(&line, &memlen, file);
    while (read != -1) {
      if (line[read - 1] == '\n') {
        line[read - 1] = 0;
      }
      pattern_add(flags, line);
      read = getline(&line, &memlen, file);
    }
    free(line);
    fclose(file);
  }
}

void opt_flag_o(regex_t *re, char *line, options flags, char *path) {
  regmatch_t match;
  int offset = 0;
  while (1) {
    int res = regexec(re, line + offset, 1, &match, 0);
    if (res != 0) {
      break;
    }
    if (flags.len_pattern > 1) printf("%s:", path);
    for (int i = match.rm_so; i < match.rm_eo; i++) {
      putchar(line[offset + i]);
      if (i == match.rm_eo - 1) puts("");
    }
    offset += match.rm_eo;
  }
}