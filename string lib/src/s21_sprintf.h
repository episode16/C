#ifndef SRC_S21_SPRINTF_H_
#define SRC_S21_SPRINTF_H_

#include "s21_string.h"

union obj {
  int64_t integer;
  uint64_t uinteger;
};

typedef struct {
  int minus;
  int plus;
  int space;
  int zero;
  int hash;
  int precision;
  int is_precision_set;
  s21_size_t width;
  char length;
  char specifier;
} flags;

const char *flag_parser(const char *input, flags *flags, va_list va);
int s21_sprintf(char *str, const char *input, ...);
const char *get_flags(const char *input, flags *flags);
const char *get_width(const char *input, flags *flags, va_list va);
const char *get_precision(const char *input, flags *flags, va_list va);
const char *get_length(const char *input, flags *flags);
void handle_value(flags flags, char *buff, va_list va);
void parse_char(flags flags, char *buff, va_list va);
void format_wchar(flags flags, char *buff, wchar_t *w_c);
void format_char(flags flags, char *buff, char c);
void parse_int(flags flags, char *buff, va_list va);
void prepend_ox(char *buff, flags flags);
void parse_pointer(flags flags, char *buff, va_list va);
void whole_num_to_string(int64_t val, char *buff, int base);
void format_precision(char *buff, flags flags);
int check_integer_specifier(char c);
void parse_float(flags flags, char *buff, va_list va);
void double_to_string(long double val, char *ret, flags flags);
void parse_mantiss(flags flags, char *buff, va_list va);
void prepend_mantiss(char *str, int pow, char sign);
void parse_float_g_G(flags flags, char *buff, va_list va);
void remove_trailing_zeroes(char *buff);
void format_gG_precision(char *buff, int precision);
void parse_string(flags flags, char *buff, va_list va);
void format_wide_string(flags flags, char *buff, wchar_t *wstr);
void format_string(flags flags, char *buff, char *str);
void parse_unsigned(flags flags, char *buff, va_list va);
void unsigned_num_to_string(uint64_t val, char *ret, int base);
void format_flags(char *buff, flags flags);
int is_all_zeroes(char *buff);
void to_upper(char *str);
void strings_handler(flags flags, char *buff, char *tmp);
void set_char(s21_size_t start_num, s21_size_t end_num, char *buff, char ch);

#endif  //  SRC_S21_SPRINTF_H_
