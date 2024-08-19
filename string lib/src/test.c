#include <check.h>
#include <ctype.h>
#include <locale.h>
#include <string.h>

#include "s21_string.h"
Suite *suite_sprintf(void);
void run_tests(void);
void run_testcase(Suite *testcase);

START_TEST(test_s21_insert_normal) {
  char *res = s21_insert("Hello, world", "beautiful", 7);
  ck_assert_str_eq(res, "Hello, beautifulworld");
  free(res);
}
END_TEST

START_TEST(test_s21_insert_at_start) {
  char *res = s21_insert("world", "Hello, ", 0);
  ck_assert_str_eq(res, "Hello, world");
  free(res);
}
END_TEST

START_TEST(test_s21_insert_at_end) {
  char *res = s21_insert("Hello", ", world", 5);
  ck_assert_str_eq(res, "Hello, world");
  free(res);
}
END_TEST

START_TEST(test_s21_insert_empty_src) {
  char *res = s21_insert("", "Hello", 0);
  ck_assert_str_eq(res, "Hello");
  free(res);
}
END_TEST

START_TEST(test_s21_insert_empty_str) {
  char *res = s21_insert("Hello", "", 5);
  ck_assert_str_eq(res, "Hello");
  free(res);
}
END_TEST

START_TEST(test_s21_insert_both_empty) {
  char *res = s21_insert("", "", 0);
  ck_assert_str_eq(res, "");
  free(res);
}
END_TEST

START_TEST(test_s21_insert_null_src) {
  char *res = s21_insert(s21_NULL, "Hello", 0);
  ck_assert_ptr_eq(res, s21_NULL);
  free(res);
}
END_TEST

START_TEST(test_s21_insert_null_str) {
  char *res = s21_insert("Hello", s21_NULL, 5);
  ck_assert_ptr_eq(res, s21_NULL);
  free(res);
}
END_TEST

START_TEST(test_s21_insert_index_out_of_bounds) {
  char *res = s21_insert("Hello", "world", 10);
  ck_assert_ptr_eq(res, s21_NULL);
  free(res);
}
END_TEST

START_TEST(test_s21_to_lower_normal) {
  char *res = s21_to_lower("Hello, World!");
  ck_assert_str_eq(res, "hello, world!");
  free(res);
}
END_TEST

START_TEST(test_s21_to_lower_empty) {
  char *res = s21_to_lower("");
  ck_assert_str_eq(res, "");
  free(res);
}
END_TEST

START_TEST(test_s21_to_lower_null) {
  char *res = s21_to_lower(s21_NULL);
  ck_assert_ptr_eq(res, s21_NULL);
  free(res);
}
END_TEST

START_TEST(test_s21_to_lower_all_uppercase) {
  char *res = s21_to_lower("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  ck_assert_str_eq(res, "abcdefghijklmnopqrstuvwxyz");
  free(res);
}
END_TEST

START_TEST(test_s21_to_lower_all_lowercase) {
  char *res = s21_to_lower("abcdefghijklmnopqrstuvwxyz");
  ck_assert_str_eq(res, "abcdefghijklmnopqrstuvwxyz");
  free(res);
}
END_TEST

START_TEST(test_s21_to_lower_mixed_characters) {
  char *res = s21_to_lower("123 ABC def %$#");
  ck_assert_str_eq(res, "123 abc def %$#");
  free(res);
}
END_TEST

START_TEST(test_s21_to_upper_normal) {
  char *res = s21_to_upper("Hello, World!");
  ck_assert_str_eq(res, "HELLO, WORLD!");
  free(res);
}
END_TEST

START_TEST(test_s21_to_upper_empty) {
  char *res = s21_to_upper("");
  ck_assert_str_eq(res, "");
  free(res);
}
END_TEST

START_TEST(test_s21_to_upper_null) {
  char *res = s21_to_upper(s21_NULL);
  ck_assert_ptr_eq(res, s21_NULL);
  free(res);
}
END_TEST

START_TEST(test_s21_to_upper_all_lowercase) {
  char *res = s21_to_upper("abcdefghijklmnopqrstuvwxyz");
  ck_assert_str_eq(res, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  free(res);
}
END_TEST

START_TEST(test_s21_to_upper_all_uppercase) {
  char *res = s21_to_upper("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  ck_assert_str_eq(res, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  free(res);
}
END_TEST

START_TEST(test_s21_to_upper_mixed_characters) {
  char *res = s21_to_upper("123 abc DEF %$#");
  ck_assert_str_eq(res, "123 ABC DEF %$#");
  free(res);
}
END_TEST

START_TEST(test_s21_trim_src_null) {
  char *res = s21_trim(NULL, NULL);
  ck_assert_ptr_eq(res, NULL);
  free(res);
}
END_TEST

START_TEST(test_s21_trim_trim_chars_null) {
  char *res = s21_trim("  Hello, World!  ", NULL);
  ck_assert_str_eq(res, "Hello, World!");
  free(res);
}
END_TEST

START_TEST(test_s21_trim_custom_trim_chars) {
  char *res = s21_trim("xxHello, World!xx", "x");
  ck_assert_str_eq(res, "Hello, World!");
  free(res);
}
END_TEST

START_TEST(test_s21_trim_only_trim_chars) {
  char *res = s21_trim("      ", NULL);
  ck_assert_str_eq(res, "");
  free(res);
}
END_TEST

START_TEST(test_s21_trim_no_trim_chars) {
  char *res = s21_trim("Hello, World!", "");
  ck_assert_str_eq(res, "Hello, World!");
  free(res);
}
END_TEST

START_TEST(test_s21_trim_normal) {
  char *res = s21_trim("  Hello, World!  ", " ");
  ck_assert_str_eq(res, "Hello, World!");
  free(res);
}
END_TEST

START_TEST(test_s21_trim_within_buffer) {
  char *res = s21_trim(" abc ", " ");
  ck_assert_str_eq(res, "abc");
  free(res);
}
END_TEST

START_TEST(test_s21_memchr) {
  char *res = s21_memchr(" abc ", ' ', 4);
  ck_assert_str_eq(res, memchr(" abc ", ' ', 4));
  res = s21_memchr("123", ' ', 4);
  ck_assert_ptr_eq(res, memchr("123", ' ', 4));
  res = s21_memchr(" ", '1', 2);
  ck_assert_ptr_eq(res, memchr(" ", '1', 2));
}
END_TEST

START_TEST(test_s21_strlen) {
  char *str = "test string";
  int a = (int)s21_strlen(str);
  ck_assert_int_eq(a, strlen(str));
  a = (int)s21_strlen("\0");
  ck_assert_int_eq(a, strlen("\0"));
}
END_TEST

START_TEST(test_s21_strtok) {
  char str[30] = "test1 / test2 / test3 / test4";
  char sep[5] = "/";
  char str2[30] = "test1 / test2 / test3 / test4";
  char *res = s21_strtok(str, sep);
  char *res2 = strtok(str2, sep);
  ck_assert_str_eq(res, res2);
  while (res != s21_NULL && res2 != NULL) {
    ck_assert_str_eq(res, res2);
    res = s21_strtok(s21_NULL, sep);
    res2 = strtok(NULL, sep);
  }

  char sep2[5] = "aaa";
  res = s21_strtok(str, sep2);
  res2 = strtok(str2, sep2);
  ck_assert_str_eq(res, res2);
}
END_TEST

START_TEST(test_s21_strstr) {
  char *res = s21_strstr("Hello world", "world");
  ck_assert_str_eq(res, strstr("Hello world", "world"));
  res = s21_strstr("Hello world", "");
  ck_assert_str_eq(res, strstr("Hello world", ""));
  res = s21_strstr("world", "hello");
  ck_assert_ptr_eq(res, strstr("world", "hello"));
}
END_TEST

START_TEST(test_s21_strrchr) {
  char *res = s21_strrchr("Hello world", 'o');
  ck_assert_str_eq(res, strrchr("Hello world", 'o'));
  res = s21_strrchr("Hello world", '3');
  ck_assert_ptr_eq(res, strrchr("Hello world", '3'));
  res = s21_strrchr("", 5);
  ck_assert_ptr_eq(res, strrchr("", 5));
}
END_TEST

START_TEST(test_s21_strpbrk) {
  char *res = s21_strpbrk("Hello world", "lle");
  ck_assert_str_eq(res, strpbrk("Hello world", "lle"));
  res = s21_strpbrk("Hello world", "aa");
  ck_assert_ptr_eq(res, strpbrk("Hello world", "aa"));
  res = s21_strpbrk("", "aa");
  ck_assert_ptr_eq(res, strpbrk("", "aa"));
}
END_TEST

START_TEST(test_s21_strcspn) {
  int res = (int)s21_strcspn("Hello world", "world");
  ck_assert_int_eq(res, (int)strcspn("Hello world", "world"));
  res = (int)s21_strcspn("Hello world", "aaa");
  ck_assert_int_eq(res, (int)strcspn("Hello world", "aaa"));
  res = (int)s21_strcspn("", "");
  ck_assert_int_eq(res, (int)strcspn("", ""));
}
END_TEST

START_TEST(test_s21_strncpy) {
  char src[5] = "hello";
  char str1[10] = "";
  char str2[10] = "";
  s21_strncpy(str1, src, 5);
  strncpy(str2, src, 5);
  ck_assert_str_eq(str1, str2);

  char res[7] = "aaa";
  char exp[7] = "aaa";
  s21_strncpy(res, src, 2);
  strncpy(exp, src, 2);
  ck_assert_str_eq(res, exp);

  int zero = 0;
  char zerosrc[] = "";
  char zerores[7] = "";
  char zeroexp[7] = "";
  s21_strncpy(zerores, zerosrc, zero);
  strncpy(zeroexp, zerosrc, zero);
  ck_assert_str_eq(zerores, zeroexp);

  char zerobyteres[7] = "";
  char zerobyteexp[7] = "";
  s21_strncpy(zerobyteres, src, zero);
  strncpy(zerobyteexp, src, zero);
  ck_assert_str_eq(zerores, zeroexp);
}
END_TEST

START_TEST(test_s21_strncat) {
  char *src = "hello";
  char dest[10] = "";
  char dest2[10] = "";
  ck_assert_str_eq(s21_strncat(dest, src, 5), strncat(dest2, src, 5));

  char dest3[10] = "";
  char dest4[10] = "";
  ck_assert_str_eq(s21_strncat(dest3, src, 3), strncat(dest4, src, 3));

  char dest5[10] = "";
  char dest6[10] = "";
  ck_assert_str_eq(s21_strncat(dest5, src, 0), strncat(dest6, src, 0));
  char *zerosrc = "";
  char dest7[10] = "";
  char dest8[10] = "";
  ck_assert_str_eq(s21_strncat(dest7, zerosrc, 4), strncat(dest8, zerosrc, 4));
}
END_TEST

START_TEST(test_s21_strncmp) {
  char *str1 = "hello";
  char *str2 = "hello";
  ck_assert_int_eq(s21_strncmp(str1, str2, 5), strncmp(str1, str2, 5));
  char *str3 = "world";
  ck_assert_int_eq(s21_strncmp(str1, str3, 5), strncmp(str1, str3, 5));
  char *str4 = "world1";
  ck_assert_int_eq(s21_strncmp(str1, str4, 5), strncmp(str1, str4, 5));
  char *str5 = "hell";
  ck_assert_int_eq(s21_strncmp(str1, str5, 5), strncmp(str1, str5, 5));
  char *str6 = "";
  ck_assert_int_eq(s21_strncmp(str1, str6, 0), strncmp(str1, str6, 0));
}
END_TEST

START_TEST(test_s21_memcmp) {
  char *str1 = "hello";
  char *str2 = "hello";
  ck_assert_int_eq(s21_memcmp(str1, str2, 5), memcmp(str1, str2, 5));
  char *str3 = "world";
  ck_assert_int_eq(s21_memcmp(str1, str3, 5), memcmp(str1, str3, 5));
  char *str4 = "world1";
  ck_assert_int_eq(s21_memcmp(str1, str4, 5), memcmp(str1, str4, 5));
  char *str5 = "hell";
  ck_assert_int_eq(s21_memcmp(str1, str5, 5), memcmp(str1, str5, 5));
  char *str6 = "";
  ck_assert_int_eq(s21_memcmp(str1, str6, 0), memcmp(str1, str6, 0));
}
END_TEST

START_TEST(test_s21_memcpy) {
  char src[5] = "hello";
  char str1[10] = "";
  char str2[10] = "";
  s21_memcpy(str1, src, 5);
  memcpy(str2, src, 5);
  ck_assert_str_eq(str1, str2);

  char res[7] = "aaa";
  char exp[7] = "aaa";
  s21_memcpy(res, src, 2);
  memcpy(exp, src, 2);
  ck_assert_str_eq(res, exp);

  char zerosrc[] = "";
  char zerores[7] = "";
  char zeroexp[7] = "";
  s21_memcpy(zerores, zerosrc, 0);
  memcpy(zeroexp, zerosrc, 0);
  ck_assert_str_eq(zerores, zeroexp);

  char zerobyteres[7] = "";
  char zerobyteexp[7] = "";
  s21_memcpy(zerobyteres, src, 0);
  memcpy(zerobyteexp, src, 0);
  ck_assert_str_eq(zerores, zeroexp);
}
END_TEST

START_TEST(test_s21_memset) {
  char str[6] = "Hello";
  char str2[6] = "Hello";
  ck_assert_str_eq(s21_memset(str, 'a', 4), memset(str2, 'a', 4));

  char str3[6] = "";
  char str4[6] = "";
  int zero = 0;
  s21_memset(str3, 'a', zero);
  memset(str4, 'a', zero);
  ck_assert_str_eq(str3, str4);
}
END_TEST

START_TEST(test_s21_strcpy) {
  char src[5] = "hello";
  char str1[10] = "";
  char str2[10] = "";
  s21_strcpy(str1, src);
  strcpy(str2, src);
  ck_assert_str_eq(str1, str2);

  char res[10] = "aaa";
  char exp[10] = "aaa";
  s21_strcpy(res, src);
  strcpy(exp, src);
  ck_assert_str_eq(res, exp);

  char zerosrc[] = "";
  char zerores[7] = "";
  char zeroexp[7] = "";
  s21_strcpy(zerores, zerosrc);
  strcpy(zeroexp, zerosrc);
  ck_assert_str_eq(zerores, zeroexp);
}
END_TEST

START_TEST(test_s21_isdigit) {
  int a = isdigit('g');
  int b = s21_isdigit('g');
  ck_assert_int_eq(a, b);

  ck_assert_int_eq(isdigit('\0'), s21_isdigit('\0'));
}
END_TEST

START_TEST(test_s21_atoi) {
  char *str = "1234";
  ck_assert_int_eq(atoi(str), s21_atoi(str));
  str = "-aaaaa+ -aa";
  ck_assert_int_eq(atoi(str), s21_atoi(str));
}
END_TEST

START_TEST(test_s21_memmove) {
  char str1[10] = "123456789";
  char str2[10] = "123456789";
  ck_assert_str_eq(memmove(str1, str1, 4), s21_memmove(str2, str2, 4));
  char str3[10] = "";
  char str4[10] = "";
  ck_assert_str_eq(memmove(str3, str4, 0), s21_memmove(str3, str4, 0));
}
END_TEST

START_TEST(test_s21_strerror) {
  for (int i = 0; i < 200; i++) {
    ck_assert_str_eq(s21_strerror(i), strerror(i));
  }
}
END_TEST

START_TEST(simple_int) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "Simple value %d";
  int val = 13;
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(precise_int) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%.5i";
  int val = 115;
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(width_int) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%5d";
  int val = 35;
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(minus_width_int) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%-5i";
  int val = 69;
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(plus_width_int) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%+12d";
  int val = 15;
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(star_width_int) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%0*d";
  int val = 85;
  ck_assert_int_eq(s21_sprintf(str1, format, 5, val),
                   sprintf(str2, format, 5, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(star_precision_int) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%0.*i";
  int val = 69;
  ck_assert_int_eq(s21_sprintf(str1, format, 5, val),
                   sprintf(str2, format, 5, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(many_flags_many_ints) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char format[] = "%0.*i %d %4.*i %013d %d";
  ck_assert_int_eq(
      s21_sprintf(str1, format, 5, 55, -10431, 13, 5311, 0, -581813581),
      sprintf(str2, format, 5, 55, -10431, 13, 5311, 0, -581813581));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(zero_precision_zero_int) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%.0d";
  int val = 0;
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(flags_long_int) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%+5.31li";
  long int val = 698518581899;
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(flags_short_int) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%-16.9hi";
  short int val = 6958;
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(unsigned_val_many_flags) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "% 5.51u";
  unsigned int val = 14140;
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST
START_TEST(unsigned_val_short) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%hu";
  unsigned short int val = 14140;
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(unsigned_val_long) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%lu";
  unsigned long int val = 949149114140;
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(unsigned_val_many) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%lu, %u, %hu, %.5u, %5.u";
  unsigned long int val = 949149114140;
  ck_assert_int_eq(
      s21_sprintf(str1, format, val, 14, 1441, 14414, 9681),
      sprintf(str2, format, val, (unsigned)14, (unsigned short)1441,
              (unsigned)14414, (unsigned)9681));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(octal_many) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%lo, %o, %ho, %.5o, %5.o";
  long int val = 949149114140;
  ck_assert_int_eq(
      s21_sprintf(str1, format, val, 14, 1441, 14414, 9681),
      sprintf(str2, format, val, (unsigned)14, (unsigned short)1441,
              (unsigned)14414, (unsigned)9681));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(hex_many) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%#-10x%x%X%#x%#X%5.5x";
  unsigned val = 858158158;
  ck_assert_int_eq(s21_sprintf(str1, format, val, val, val, val, val, val),
                   sprintf(str2, format, val, val, val, val, val, val));
  ck_assert_str_eq(str1, str2);
}

END_TEST
START_TEST(hex_many_flags) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%#-5.10x";
  unsigned val = 858158158;
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(hex_zero) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%#x";
  unsigned val = 0;
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(one_many_flags) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%-5.3c";
  char val = 'c';
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(one_width) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%15c";
  char val = 'c';
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(one_many) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%-5.3c%c%c%c%c Hello! ABOBA";
  char val = 'c';
  ck_assert_int_eq(s21_sprintf(str1, format, val, 'c', 'a', 'b', 'b'),
                   sprintf(str2, format, val, 'c', 'a', 'b', 'b'));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(string_precision) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%.15s";
  char *val = "I LOVE STRINGS AND TESTS AND SCHOOL21 NO JOKE";
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));

  ck_assert_str_eq(str1, str2);
}

START_TEST(string_flags) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%-15.9s";
  char *val = "69 IS MY FAVORITE NUMBER";
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(ptr) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%p";
  ck_assert_int_eq(s21_sprintf(str1, format, format),
                   sprintf(str2, format, format));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(null_ptr) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%p";
  char *ptr = NULL;
  s21_sprintf(str1, format, ptr);
  sprintf(str2, format, ptr);
  ck_assert_int_eq(s21_sprintf(str1, format, ptr), sprintf(str2, format, ptr));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(string_width_huge) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *val = "kjafdiuhfjahfjdahf";
  char *format = "%120s";
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(ptr_width) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%15p";
  ck_assert_int_eq(s21_sprintf(str1, format, format),
                   sprintf(str2, format, format));
  ck_assert_str_eq(str1, str2);
}
END_TEST
START_TEST(ptr_precision) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%.5p";
  ck_assert_int_eq(s21_sprintf(str1, format, format),
                   sprintf(str2, format, format));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(float_precision) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%Lf";
  long double val = 513515.131513515151351;
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST
START_TEST(float_width) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%10Lf";
  long double val = 15.35;
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST
START_TEST(float_precision_zero) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%.0Lf";
  long double val = 15.35;
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(float_many) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "% .0f %.lf %Lf %f %lf %Lf";
  float val = 0;
  double val1 = 0;
  long double val2 = 3515315.153151;
  float val3 = 5.5;
  double val4 = 9851.51351;
  long double val5 = 95919539159.53151351131;
  ck_assert_int_eq(s21_sprintf(str1, format, val, val1, val2, val3, val4, val5),
                   sprintf(str2, format, val, val1, val2, val3, val4, val5));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(e_precision) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%.17Le";
  long double val = 15.35;
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST
START_TEST(e_precision_zero) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "%.0Le";
  long double val = 15.35;
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(e_many) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char *format = "% .0e %.le %Le %e %le %Le";
  float val = 0;
  double val1 = 0;
  long double val2 = 3515315.153151;
  float val3 = 5.5;
  double val4 = 0.094913941;
  long double val5 = 95919539159.53151351131;
  ck_assert_int_eq(s21_sprintf(str1, format, val, val1, val2, val3, val4, val5),
                   sprintf(str2, format, val, val1, val2, val3, val4, val5));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(test_many_float) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  ck_assert_int_eq(
      s21_sprintf(str1, "%f%f%f%f", -999.666, 0.0001, 666.999, -100.001),
      sprintf(str2, "%f%f%f%f", -999.666, 0.0001, 666.999, -100.001));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(test_one_unsigned_dec) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];

  ck_assert_int_eq(s21_sprintf(str1, "%u", 100),
                   sprintf(str2, "%u", (unsigned)100));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(wide_char) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  setlocale(LC_ALL, "");
  char *format = "This is a simple wide char %lc";
  unsigned long w = L'☺';
  int a = s21_sprintf(str1, format, w);
  int b = sprintf(str2, format, w);
  ck_assert_str_eq(str1, str2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(width_char) {
  char str1[BUFFER_SIZE] = {'\0'};
  char str2[BUFFER_SIZE] = {'\0'};
  char *format = "This is a simple wide char %c";
  char w = 'c';
  int a = s21_sprintf(str1, format, w);
  int b = sprintf(str2, format, w);

  ck_assert_str_eq(str1, str2);
  ck_assert_int_eq(a, b);
}
END_TEST
START_TEST(minus_wide_char) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  setlocale(LC_ALL, "");
  char *format = "This is a simple wide char %-5lc";
  unsigned long w = L'☺';
  int a = s21_sprintf(str1, format, w);
  int b = sprintf(str2, format, w);
  ck_assert_str_eq(str1, str2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(whith_out_minus_wide_char) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  setlocale(LC_ALL, "");
  char *format = "This is a simple wide char %5lc";
  unsigned long w = L'☺';
  int a = s21_sprintf(str1, format, w);
  int b = sprintf(str2, format, w);
  ck_assert_str_eq(str1, str2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(only_minus_wide_char) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  setlocale(LC_ALL, "");
  char *format = "This is a simple wide char %-lc";
  unsigned long w = L'☺';
  int a = s21_sprintf(str1, format, w);
  int b = sprintf(str2, format, w);
  ck_assert_str_eq(str1, str2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(wide_string) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  setlocale(LC_ALL, "");
  char *format = "This is a simple wide char %ls";
  wchar_t w[] = L"☺";
  int a = s21_sprintf(str1, format, w);
  int b = sprintf(str2, format, w);
  ck_assert_str_eq(str1, str2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(g_trailing_zero) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char format[] = "%g";
  double hex = 0.50300;
  ck_assert_int_eq(s21_sprintf(str1, format, hex), sprintf(str2, format, hex));

  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(g_large) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char format[] = "%g";
  double hex = 5131.43141;
  ck_assert_int_eq(s21_sprintf(str1, format, hex), sprintf(str2, format, hex));

  ck_assert_str_eq(str1, str2);
}
END_TEST
START_TEST(g_small) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char format[] = "%g";
  double hex = 0.123000;
  ck_assert_int_eq(s21_sprintf(str1, format, hex), sprintf(str2, format, hex));

  ck_assert_str_eq(str1, str2);
}
END_TEST
START_TEST(g_precision) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char format[] = "%.5g";
  double hex = 0.123000;
  ck_assert_int_eq(s21_sprintf(str1, format, hex), sprintf(str2, format, hex));

  ck_assert_str_eq(str1, str2);
}
END_TEST
START_TEST(g_precision_zero) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char format[] = "%.0g";
  double hex = 0.123000;
  ck_assert_int_eq(s21_sprintf(str1, format, hex), sprintf(str2, format, hex));

  ck_assert_str_eq(str1, str2);
}
END_TEST
START_TEST(g_precision_missing) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char format[] = "%.g";
  double hex = 0.123000;
  ck_assert_int_eq(s21_sprintf(str1, format, hex), sprintf(str2, format, hex));

  ck_assert_str_eq(str1, str2);
}
END_TEST
START_TEST(g_many_zeroes_in_front) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char format[] = "%.g";
  double hex = 0.0004;
  ck_assert_int_eq(s21_sprintf(str1, format, hex), sprintf(str2, format, hex));

  ck_assert_str_eq(str1, str2);
}
END_TEST
START_TEST(g_one_zero) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char format[] = "%g";
  double hex = 0;
  ck_assert_int_eq(s21_sprintf(str1, format, hex), sprintf(str2, format, hex));

  ck_assert_str_eq(str1, str2);
}
END_TEST
START_TEST(g_mantiss_flags) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char format[] = "%5.8g";
  double hex = 0.0000005;
  ck_assert_int_eq(s21_sprintf(str1, format, hex), sprintf(str2, format, hex));

  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(LG) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char format[] = "%LG";
  long double hex = 0.000005;
  ck_assert_int_eq(s21_sprintf(str1, format, hex), sprintf(str2, format, hex));

  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(g_many) {
  char str1[BUFFER_SIZE];
  char str2[BUFFER_SIZE];
  char format[] = "%LG %g %G %Lg %.5g";
  long double hex = 0.000005;
  double hex1 = 41.1341;
  double hex2 = 848.9000;
  long double hex3 = 0.0843;
  double hex4 = 0.0005;
  double hex5 = 0.8481481;
  ck_assert_int_eq(s21_sprintf(str1, format, hex, hex1, hex2, hex3, hex4, hex5),
                   sprintf(str2, format, hex, hex1, hex2, hex3, hex4, hex5));

  ck_assert_str_eq(str1, str2);
}
END_TEST

Suite *suite_sprintf(void) {
  Suite *s = suite_create("suite_sprintf");
  TCase *tc = tcase_create("sprintf_tc");
  tcase_add_test(tc, simple_int);
  tcase_add_test(tc, precise_int);
  tcase_add_test(tc, width_int);
  tcase_add_test(tc, minus_width_int);
  tcase_add_test(tc, plus_width_int);
  tcase_add_test(tc, star_width_int);
  tcase_add_test(tc, star_precision_int);
  tcase_add_test(tc, many_flags_many_ints);
  tcase_add_test(tc, flags_long_int);
  tcase_add_test(tc, flags_short_int);
  tcase_add_test(tc, zero_precision_zero_int);
  tcase_add_test(tc, unsigned_val_many_flags);
  tcase_add_test(tc, unsigned_val_short);
  tcase_add_test(tc, unsigned_val_long);
  tcase_add_test(tc, unsigned_val_many);
  tcase_add_test(tc, octal_many);
  tcase_add_test(tc, hex_many);
  tcase_add_test(tc, hex_many_flags);
  tcase_add_test(tc, hex_zero);
  tcase_add_test(tc, one_width);
  tcase_add_test(tc, one_many);
  tcase_add_test(tc, string_precision);
  tcase_add_test(tc, string_flags);
  tcase_add_test(tc, ptr);
  tcase_add_test(tc, ptr_width);
  tcase_add_test(tc, ptr_precision);
  tcase_add_test(tc, null_ptr);
  tcase_add_test(tc, float_precision);
  tcase_add_test(tc, float_width);
  tcase_add_test(tc, float_many);
  tcase_add_test(tc, e_precision);
  tcase_add_test(tc, e_precision_zero);
  tcase_add_test(tc, e_many);
  tcase_add_test(tc, test_many_float);
  tcase_add_test(tc, test_one_unsigned_dec);
  tcase_add_test(tc, wide_char);
  tcase_add_test(tc, width_char);
  tcase_add_test(tc, minus_wide_char);
  tcase_add_test(tc, wide_string);
  tcase_add_test(tc, float_precision_zero);
  tcase_add_test(tc, string_width_huge);
  tcase_add_test(tc, one_many_flags);
  tcase_add_test(tc, whith_out_minus_wide_char);
  tcase_add_test(tc, g_trailing_zero);
  tcase_add_test(tc, g_large);
  tcase_add_test(tc, g_precision);
  tcase_add_test(tc, g_precision_zero);
  tcase_add_test(tc, g_precision_missing);
  tcase_add_test(tc, g_many_zeroes_in_front);
  tcase_add_test(tc, g_one_zero);
  tcase_add_test(tc, g_mantiss_flags);
  tcase_add_test(tc, LG);
  tcase_add_test(tc, g_many);
  tcase_add_test(tc, g_small);
  tcase_add_test(tc, only_minus_wide_char);
  tcase_add_test(tc, test_s21_insert_normal);
  tcase_add_test(tc, test_s21_insert_at_start);
  tcase_add_test(tc, test_s21_insert_at_end);
  tcase_add_test(tc, test_s21_insert_empty_src);
  tcase_add_test(tc, test_s21_insert_empty_str);
  tcase_add_test(tc, test_s21_insert_both_empty);
  tcase_add_test(tc, test_s21_insert_null_src);
  tcase_add_test(tc, test_s21_insert_null_str);
  tcase_add_test(tc, test_s21_insert_index_out_of_bounds);
  tcase_add_test(tc, test_s21_to_lower_normal);
  tcase_add_test(tc, test_s21_to_lower_empty);
  tcase_add_test(tc, test_s21_to_lower_null);
  tcase_add_test(tc, test_s21_to_lower_all_uppercase);
  tcase_add_test(tc, test_s21_to_lower_all_lowercase);
  tcase_add_test(tc, test_s21_to_lower_mixed_characters);
  tcase_add_test(tc, test_s21_to_upper_normal);
  tcase_add_test(tc, test_s21_to_upper_empty);
  tcase_add_test(tc, test_s21_to_upper_null);
  tcase_add_test(tc, test_s21_to_upper_all_lowercase);
  tcase_add_test(tc, test_s21_to_upper_all_uppercase);
  tcase_add_test(tc, test_s21_to_upper_mixed_characters);
  tcase_add_test(tc, test_s21_trim_src_null);
  tcase_add_test(tc, test_s21_trim_trim_chars_null);
  tcase_add_test(tc, test_s21_trim_custom_trim_chars);
  tcase_add_test(tc, test_s21_trim_only_trim_chars);
  tcase_add_test(tc, test_s21_trim_no_trim_chars);
  tcase_add_test(tc, test_s21_trim_normal);
  tcase_add_test(tc, test_s21_trim_within_buffer);
  tcase_add_test(tc, test_s21_strlen);
  tcase_add_test(tc, test_s21_memchr);
  tcase_add_test(tc, test_s21_strtok);
  tcase_add_test(tc, test_s21_strstr);
  tcase_add_test(tc, test_s21_strrchr);
  tcase_add_test(tc, test_s21_strpbrk);
  tcase_add_test(tc, test_s21_strcspn);
  tcase_add_test(tc, test_s21_strncpy);
  tcase_add_test(tc, test_s21_strncat);
  tcase_add_test(tc, test_s21_strncmp);
  tcase_add_test(tc, test_s21_memcmp);
  tcase_add_test(tc, test_s21_memcpy);
  tcase_add_test(tc, test_s21_memset);
  tcase_add_test(tc, test_s21_strcpy);
  tcase_add_test(tc, test_s21_isdigit);
  tcase_add_test(tc, test_s21_atoi);
  tcase_add_test(tc, test_s21_memmove);
  tcase_add_test(tc, test_s21_strerror);
  suite_add_tcase(s, tc);
  return s;
}

int main() {
  int number_failed;
  Suite *s;
  SRunner *sr;
  s = suite_sprintf();
  sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
