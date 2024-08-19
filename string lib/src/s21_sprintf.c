#include "s21_sprintf.h"

int s21_sprintf(char *output, const char *input, ...) {
  flags flags = {0};
  va_list va;
  va_start(va, input);
  char *output_beginning = output;
  while (*input) {
    if (*input != '%') {
      *output++ = *input++;
      continue;
    }
    if (*input++ == '%') {
      char buff[BUFFER_SIZE] = {'\0'};
      s21_memset(&flags, 0, sizeof(flags));
      input = flag_parser(input, &flags, va);
      input++;
      handle_value(flags, buff, va);
      for (int i = 0; buff[i]; i++, output++) *output = buff[i];
    }
  }
  *output = '\0';
  va_end(va);
  return output - output_beginning;
}

const char *flag_parser(const char *input, flags *flags, va_list va) {
  input = get_flags(input, flags);
  input = get_width(input, flags, va);
  input = get_precision(input, flags, va);
  input = get_length(input, flags);
  flags->specifier = *input;
  return input;
}

const char *get_flags(const char *input, flags *flags) {
  while (*input == '-' || *input == '+' || *input == ' ' || *input == '0' ||
         *input == '#') {
    switch (*input) {
      case '0':
        flags->zero = 1;
        break;
      case '-':
        flags->minus = 1;
        break;
      case '+':
        flags->plus = 1;
        break;
      case ' ':
        flags->space = 1;
        break;
      case '#':
        flags->hash = 1;
        break;
    }
    input++;
  }
  return input;
}

const char *get_width(const char *input, flags *flags, va_list va) {
  if (*input == '*') {
    input++;
    flags->width = va_arg(va, int);
  }
  if (s21_isdigit(*input)) {
    char tmp[BUFFER_SIZE] = {'\0'};
    for (int i = 0; s21_isdigit(*input); i++, input++) tmp[i] = *input;
    flags->width = s21_atoi(tmp);
  }
  return input;
}

const char *get_precision(const char *input, flags *flags, va_list va) {
  if (*input == '.') {
    flags->is_precision_set = 1;
    input++;
  }
  if (*input == '*') {
    input++;
    flags->precision = va_arg(va, int);
  }
  if (s21_isdigit(*input)) {
    char tmp[BUFFER_SIZE] = {'\0'};
    for (int i = 0; s21_isdigit(*input); i++, input++) tmp[i] = *input;
    flags->precision = s21_atoi(tmp);
  }
  return input;
}

const char *get_length(const char *input, flags *flags) {
  switch (*input) {
    case 'h':
      flags->length = 'h';
      input++;
      break;
    case 'l':
      flags->length = 'l';
      input++;
      break;
    case 'L':
      flags->length = 'L';
      input++;
      break;
  }
  return input;
}

void handle_value(flags flags, char *buff, va_list va) {
  if (flags.specifier == 'c') parse_char(flags, buff, va);
  if (flags.specifier == 'd' || flags.specifier == 'i' ||
      flags.specifier == 'o')
    parse_int(flags, buff, va);
  if (flags.specifier == 'f') parse_float(flags, buff, va);
  if (flags.specifier == 's') parse_string(flags, buff, va);
  if (flags.specifier == '%') buff[0] = '%';
  if (flags.specifier == 'u' || flags.specifier == 'x' ||
      flags.specifier == 'X' || flags.specifier == 'p')
    parse_unsigned(flags, buff, va);
  if (flags.specifier == 'e' || flags.specifier == 'E')
    parse_mantiss(flags, buff, va);
  if (flags.specifier == 'g' || flags.specifier == 'G')
    parse_float_g_G(flags, buff, va);
  if (flags.specifier == 'G' || flags.specifier == 'E' ||
      flags.specifier == 'X')
    to_upper(buff);
}

void parse_char(flags flags, char *buff, va_list va) {
  if (flags.length == 'l') {
    wchar_t w_c = L'Ф';
    w_c = va_arg(va, wchar_t);
    format_wchar(flags, buff, &w_c);
  } else {
    char c = 'a';
    c = va_arg(va, int);
    format_char(flags, buff, c);
  }
}

void format_wchar(flags flags, char *buff, wchar_t *w_c) {
  char tmp[BUFFER_SIZE] = {'\0'};
  if (!flags.minus) {
    wcstombs(tmp, w_c, BUFFER_SIZE);
    s21_size_t tmp_len = s21_strlen(tmp);
    if (flags.width > 0) set_char(0, flags.width - tmp_len, buff, ' ');
    s21_strncat(buff, tmp, BUFFER_SIZE);
  } else if (flags.width) {
    wcstombs(buff, w_c, BUFFER_SIZE);
    s21_size_t buff_len = s21_strlen(buff);
    set_char(buff_len, flags.width, buff, ' ');
  } else
    wcstombs(buff, w_c, BUFFER_SIZE);
}

void format_char(flags flags, char *buff, char c) {
  if (!flags.minus && flags.width) {
    set_char(0, flags.width, buff, ' ');
    buff[flags.width - 1] = c;
  } else if (flags.width) {
    buff[0] = c;
    set_char(1, flags.width, buff, ' ');
  } else
    buff[0] = c;
}

void set_char(s21_size_t start_num, s21_size_t end_num, char *buff, char ch) {
  for (s21_size_t i = start_num; i < end_num; i++) buff[i] = ch;
}

void parse_int(flags flags, char *buff, va_list va) {
  int64_t val = va_arg(va, int64_t);
  if (flags.specifier == 'o') {
    buff[0] = '0';
    whole_num_to_string(val, buff + flags.hash, 8);
  } else {
    switch (flags.length) {
      case 0:
        val = (int32_t)val;
        break;
      case 'h':
        val = (int16_t)val;
    }
    whole_num_to_string(val, buff, 10);
  }
  format_precision(buff, flags);
  format_flags(buff, flags);
}

void parse_unsigned(flags flags, char *buff, va_list va) {
  uint64_t val = va_arg(va, uint64_t);
  if (flags.specifier == 'p') {
    unsigned_num_to_string(val, buff, 16);
  } else {
    switch (flags.length) {
      case 'h':
        val = (uint16_t)val;
        break;
      case 0:
        val = (uint32_t)val;
    }
    if (flags.specifier == 'x' || flags.specifier == 'X') {
      unsigned_num_to_string(val, buff, 16);
    } else {
      unsigned_num_to_string(val, buff, 10);
    }
  }
  format_precision(buff, flags);
  if ((flags.hash && (flags.specifier == 'x' || flags.specifier == 'X')) ||
      flags.specifier == 'p')
    prepend_ox(buff, flags);
  format_flags(buff, flags);
}

void prepend_ox(char *buff, flags flags) {
  s21_size_t len = s21_strlen(buff);
  if (!is_all_zeroes(buff) || flags.specifier == 'p') {
    s21_memmove(buff + 2, buff, len);
    buff[0] = '0';
    buff[1] = 'x';
  }
}
void whole_num_to_string(int64_t val, char *buff, int base) {
  char tmp[BUFFER_SIZE] = {'\0'};
  int idx = BUFFER_SIZE - 2;
  int flag_neg = val < 0 ? 1 : 0;
  val = val < 0 ? -val : val;
  if (val == 0) tmp[idx] = '0';
  while (val > 0) {
    idx--;
    tmp[idx] = "0123456789abcdef"[val % base];
    val /= base;
  }

  for (int j = 0; tmp[idx]; idx++, j++) {
    if (flag_neg && j == 0) buff[j++] = '-';
    buff[j] = tmp[idx];
  }
}

void format_precision(char *buff, flags flags) {
  char tmp[BUFFER_SIZE] = {'\0'};
  int sign = 0;
  int len = s21_strlen(buff);
  if (buff[0] == '-') {
    tmp[0] = '-';
    len--;
    sign = 1;
  }
  if (flags.precision > len) {
    int idx;
    for (idx = sign; idx < flags.precision - len + sign; idx++) tmp[idx] = '0';
    for (int i = sign; buff[i]; i++, idx++) tmp[idx] = buff[i];

    s21_strncpy(buff, tmp, s21_strlen(tmp));
  }
  if (flags.is_precision_set && flags.precision == 0 &&
      check_integer_specifier(flags.specifier) && buff[0] == '0')
    buff[0] = '\0';
}

int check_integer_specifier(char c) {
  char specs[] = {'d', 'i', 'o', 'u', 'x', 'X'};
  int res = 0;
  for (size_t i = 0; i < sizeof(specs); i++) {
    if (specs[i] == c) {
      res = 1;
      break;
    }
  }
  return res;
}

void parse_float(flags flags, char *buff, va_list va) {
  long double val = 0;
  if (flags.length == 'L') {
    val = va_arg(va, long double);
  } else {
    val = va_arg(va, double);
  }
  if (!flags.is_precision_set) flags.precision = 6;
  double_to_string(val, buff, flags);
  format_flags(buff, flags);
}

void parse_mantiss(flags flags, char *buff, va_list va) {
  long double val = 0;
  if (flags.length == 'L')
    val = va_arg(va, long double);

  else
    val = va_arg(va, double);

  int pow = 0;
  char sign = (int)val == 0 ? '-' : '+';
  if ((int)val - val) {
    while ((int)val == 0) {
      pow++;
      val *= 10;
    }
  } else
    sign = '+';

  while ((int)val / 10 != 0) {
    pow++;
    val /= 10;
  }

  if (!flags.is_precision_set) flags.precision = 6;
  double_to_string(val, buff, flags);
  prepend_mantiss(buff, pow, sign);
  format_flags(buff, flags);
}

void prepend_mantiss(char *str, int pow, char sign) {
  int len = s21_strlen(str);
  str[len] = 'e';
  str[len + 1] = sign;
  str[len + 3] = pow % 10 + '0';
  pow /= 10;
  str[len + 2] = pow % 10 + '0';
  str[len + 4] = '\0';
}

void parse_float_g_G(flags flags, char *buff, va_list va) {
  long double val = 0;
  if (flags.length == 'L')
    val = va_arg(va, long double);
  else
    val = va_arg(va, double);

  if (!flags.is_precision_set) flags.precision = 6;
  if (flags.precision == 0) flags.precision = 1;
  int precision = flags.precision;
  long double m_val = val;
  int pow = 0;
  if ((int)val - val) {
    while ((int)m_val == 0) {
      pow++;
      m_val *= 10;
    }
  }
  if (pow > 4) {
    flags.precision = 0;
    double_to_string(m_val, buff, flags);
  } else {
    flags.precision = 10;
    double_to_string(val, buff, flags);
  }
  format_gG_precision(buff, precision);
  if (pow > 4) prepend_mantiss(buff, pow, '-');
  remove_trailing_zeroes(buff);
  format_flags(buff, flags);
}

void remove_trailing_zeroes(char *buff) {
  int len = s21_strlen(buff);
  char *dot = s21_strchr(buff, '.');
  if (dot) {
    for (int i = len - 1; buff[i] != '.'; i--) {
      if (buff[i] == '0')
        buff[i] = '\0';

      else
        break;
    }
    if (dot[1] == '\0') dot[0] = '\0';
  }
}

void format_gG_precision(char *buff, int precision) {
  int sig_digs = 0;
  s21_size_t len = s21_strlen(buff);
  int not_zero_found = 0;
  for (s21_size_t i = 0; i < len; i++) {
    if ((buff[i] == '0' && !not_zero_found) || buff[i] == '.')
      continue;
    else
      not_zero_found = 1;
    if (s21_isdigit(buff[i]) && not_zero_found) sig_digs++;
    if (sig_digs == precision && i + 1 < len) {
      int next = buff[i + 1] == '.' ? 2 : 1;
      buff[i] = buff[i + next] - '0' > 5 ? (char)(buff[i] + 1) : buff[i];
      buff[i + 1] = '\0';
      break;
    }
  }
}

void double_to_string(long double val, char *ret, flags flags) {
  char buff[BUFFER_SIZE] = {'\0'};
  int idx = BUFFER_SIZE - 2;
  int neg = val < 0 ? 1 : 0;
  val = neg ? val * -1 : val;
  long double l = 0, r = modfl(val, &l);
  if (flags.precision == 0) {
    l = roundl(val);
    r = 0;
  }
  char fractions[BUFFER_SIZE] = {'\0'};
  for (int i = 0; i < flags.precision; i++) {
    r = r * 10;
    fractions[i] = (int)r + '0';
  }
  s21_size_t fractions_len = s21_strlen(fractions);
  long long right = roundl(r), left = l;
  if (!right) {
    for (int i = 0; i < flags.precision; idx--, i++) buff[idx] = '0';
  } else {
    for (int i = fractions_len; right || i > 0; right /= 10, idx--, i--)
      buff[idx] = (int)(right % 10 + 0.05) + '0';
  }
  if ((flags.is_precision_set && flags.precision != 0) || (int)r ||
      (!flags.is_precision_set && val == 0) || fractions_len)
    buff[idx--] = '.';
  if (!left) {
    buff[idx] = '0';
    idx--;
  } else {
    for (; left; left /= 10, idx--) buff[idx] = (int)(left % 10) + '0';
  }
  for (int i = 0; buff[idx + 1]; idx++, i++) {
    if (neg && i == 0) {
      ret[i] = '-';
      i++;
    }
    ret[i] = buff[idx + 1];
  }
}

void parse_string(flags flags, char *buff, va_list va) {
  char tmp[BUFFER_SIZE] = {'\0'};
  if (flags.length == 'l') {
    wchar_t *wstr = va_arg(va, wchar_t *);
    char str[BUFFER_SIZE] = {'\0'};
    wcstombs(str, wstr, BUFFER_SIZE);
    s21_strncpy(tmp, str, BUFFER_SIZE);
    strings_handler(flags, buff, tmp);
  } else {
    char *str = va_arg(va, char *);
    s21_strncpy(tmp, str, BUFFER_SIZE);
    strings_handler(flags, buff, tmp);
  }
}

void strings_handler(flags flags, char *buff, char *tmp) {
  if (flags.is_precision_set) tmp[flags.precision] = '\0';
  int shift = flags.width - s21_strlen(tmp);
  int len = s21_strlen(tmp);
  if (flags.minus && shift > 0) {
    s21_strncpy(buff, tmp, BUFFER_SIZE);
    s21_memset(buff + len, ' ', shift);
  } else if (shift > 0) {
    s21_memset(buff, ' ', shift);
    s21_strncpy(buff + shift, tmp, BUFFER_SIZE);
  } else
    s21_strncpy(buff, tmp, BUFFER_SIZE);
}

void unsigned_num_to_string(uint64_t val, char *ret, int base) {
  char buf[BUFFER_SIZE + 1] = {'\0'};
  int idx = BUFFER_SIZE - 1;
  if (val == 0) {
    buf[idx] = '0';
    idx--;
  }
  for (; val && idx; --idx, val /= base)
    buf[idx] = "0123456789abcdef"[val % base];
  for (int j = 0; buf[idx + 1]; idx++, j++) ret[j] = buf[idx + 1];
}

void format_flags(char *buff, flags flags) {
  char tmp[BUFFER_SIZE + 1] = {'\0'};
  if (flags.plus && flags.specifier != 'u') {
    tmp[0] = buff[0] == '-' ? buff[0] : '+';
    s21_strncpy(tmp + 1, buff[0] == '-' ? buff + 1 : buff, BUFFER_SIZE + 1);
    s21_strncpy(buff, tmp, BUFFER_SIZE);
  } else if (flags.space && buff[0] != '-' && flags.specifier != 'u') {
    tmp[0] = ' ';
    s21_strncpy(tmp + 1, buff, BUFFER_SIZE + 1);
    s21_strncpy(buff, tmp, BUFFER_SIZE);
  }
  if (flags.width > s21_strlen(buff)) {
    int idx = flags.width - s21_strlen(buff);
    if (!flags.minus) {
      s21_memset(tmp, flags.zero ? '0' : ' ', idx);
      s21_strncpy(tmp + idx, buff, BUFFER_SIZE + 1 - idx);
    } else {
      s21_strncpy(tmp, buff, BUFFER_SIZE + 1);
      s21_memset(tmp + s21_strlen(tmp), ' ', idx);
    }
    s21_strncpy(buff, tmp, BUFFER_SIZE);
  }
}

int is_all_zeroes(char *buff) {
  int flag = 1;
  for (int i = 0; buff[i]; i++)
    if (buff[i] != '0') flag = 0;
  return flag;
}

void to_upper(char *str) {
  while (*str) {
    if (*str >= 'a' && *str <= 'z') *str = *str - 'a' + 'A';
    str++;
  }
}
