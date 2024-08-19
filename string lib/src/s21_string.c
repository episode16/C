
#include "s21_string.h"

char *s21_strtok(char *str, const char *delim) {
  static char *last = s21_NULL;
  char *start = s21_NULL;
  if (str) last = str;
  if ((last != 0) || (*last != 0)) {
    char *c = last;
    if (*c != 0) {
      start = c;
      while (*c > 0 && (s21_strchr(delim, *c) == 0)) ++c;
    }
    *c = 0;
    last = c + 1;
  }
  return start;
}

char *s21_strstr(const char *haystack, const char *needle) {
  char *res = s21_NULL;
  if (s21_strlen(haystack) >= s21_strlen(needle)) {
    for (s21_size_t i = 0; i <= s21_strlen(haystack) - s21_strlen(needle);
         i++) {
      int found = 1;
      for (s21_size_t j = i, k = 0; needle[k]; k++, j++) {
        if (haystack[j] != needle[k]) {
          found = 0;
          break;
        }
      }
      if (found) {
        res = (char *)haystack + i;
        break;
      }
    }
  }
  return res;
}

char *s21_strrchr(const char *str, int c) {
  char *res = s21_NULL;
  s21_size_t len = s21_strlen(str);

  for (int i = len; i >= 0; i--) {
    if (str[i] == c) {
      res = ((char *)str) + i;
      break;
    }
  }

  return res;
}

char *s21_strchr(const char *str, int c) {
  char *s = s21_NULL;

  for (s21_size_t i = 0; i <= s21_strlen(str); i++) {
    if (str[i] == c) {
      s = ((char *)str) + i;
      break;
    }
  }

  return s;
}

char *s21_strpbrk(const char *str1, const char *str2) {
  const char *ptr;
  int match_found = 0;
  char *result = s21_NULL;
  while (*str1 && !match_found) {
    ptr = str2;
    while (*ptr && !match_found) {
      if (*str1 == *ptr) {
        match_found = 1;
        result = (char *)str1;
      }
      ptr++;
    }
    if (!match_found) {
      str1++;
    }
  }
  return result;
}

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t len1 = s21_strlen(str1);
  s21_size_t len2 = s21_strlen(str2);
  s21_size_t count = len1;
  for (s21_size_t i = 0; i < len1; i++) {
    for (s21_size_t j = 0; j < len2; j++) {
      if (str1[i] == str2[j]) {
        count = i;
        break;
      }
    }
    if (count == i) break;
  }
  return count;
}

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  for (s21_size_t i = 0; i < n; i++) {
    dest[i] = src[i];
    if (src[i] == '\0') break;
  }
  return dest;
}

char *s21_strncat(char *dest, char *append, s21_size_t n) {
  int len2 = s21_strlen(dest);
  s21_size_t i = 0;
  while (append[i] != '\0') {
    if (i == n) break;
    dest[len2] = append[i];
    i++;
    len2++;
  }
  dest[len2] = '\0';
  return dest;
}

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  int result = 0;

  for (s21_size_t i = 0; i < n; i++) {
    result = ((char *)str1)[i] - ((char *)str2)[i];
    if (result != 0) break;
  }
  return result;
}

s21_size_t s21_strlen(const char *str) {
  s21_size_t count = 0;
  while (str[count] != '\0') {
    count++;
  }
  return count;
}
void *s21_memchr(const void *str, int c, s21_size_t n) {
  unsigned char *ptr = (unsigned char *)str;
  void *out = s21_NULL;
  for (s21_size_t i = 0; i < n && ptr; i++) {
    if (*ptr == (unsigned char)c) {
      out = ptr;
      break;
    }
    ptr++;
  }
  return out;
}

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  int result = 0;
  for (s21_size_t i = 0; i < n; i++) {
    if (((char *)str1)[i] != ((char *)str2)[i])
      result = ((char *)str1)[i] - ((char *)str2)[i];
    if (result != 0) break;
  }
  return result;
}

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  for (s21_size_t i = 0; i < n; i++) {
    ((char *)dest)[i] = ((char *)src)[i];
  }
  return dest;
}

void *s21_memset(void *str, int c, s21_size_t n) {
  for (s21_size_t i = 0; i < n; i++) {
    ((char *)str)[i] = c;
  }
  return str;
}

char *s21_strcpy(char *dest, const char *src) {
  for (int i = 0; src[i]; i++) {
    dest[i] = src[i];
  }
  dest[s21_strlen(src)] = '\0';
  return dest;
}

int s21_isdigit(char c) { return (c >= '0' && c <= '9'); }

int s21_atoi(const char *str) {
  int res = 0;
  int sign = 1;
  int overflow = 0;
  while (*str) {
    if (*str == '-') sign = -1;
    if (*str == ' ' || *str == '+') {
      str++;
    }
    if (*str >= '0' && *str <= '9') res = res * 10 + (*str - '0');
    if (res < 0) {
      overflow = 1;
      break;
    }
    str++;
  }
  if (overflow) {
    res = sign > 0 ? INT32_MAX : INT32_MIN;
  } else {
    res *= sign;
  }
  return res;
}

void *s21_memmove(void *dest, const void *src, s21_size_t n) {
  char *to = (char *)dest;
  char *from = (char *)src;
  char *tmp = (char *)calloc(sizeof(char), n);
  if (tmp && tmp != s21_NULL) {
    s21_memcpy(tmp, from, n);
    s21_memcpy(to, tmp, n);
    free(tmp);
  }
  return dest;
}

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  void *result = s21_NULL;

  if (src && str && start_index <= s21_strlen(src)) {
    s21_size_t src_len = s21_strlen(src);
    s21_size_t str_len = s21_strlen(str);
    s21_size_t new_len = src_len + str_len;

    result = (char *)calloc((new_len + 1), sizeof(char));
    if (result) {
      s21_memcpy(result, src, start_index);
      s21_memcpy((char *)result + start_index, str, str_len);
      s21_memcpy((char *)result + start_index + str_len, src + start_index,
                 src_len - start_index);
      ((char *)result)[new_len] = '\0';
    }
  }

  return result;
}

void *s21_to_lower(const char *str) {
  char *result = s21_NULL;

  if (str) {
    s21_size_t length = 0;
    while (str[length] != '\0') {
      length++;
    }

    result = (char *)calloc(length + 1, sizeof(char));
    if (result) {
      for (s21_size_t i = 0; i < length; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
          result[i] = str[i] + 'a' - 'A';
        } else {
          result[i] = str[i];
        }
      }
    }
  }

  return result;
}

void *s21_to_upper(const char *str) {
  char *result = s21_NULL;

  if (str) {
    s21_size_t length = 0;
    while (str[length] != '\0') {
      length++;
    }

    result = (char *)calloc(length + 1, sizeof(char));
    if (result) {
      for (s21_size_t i = 0; i < length; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
          result[i] = str[i] - 'a' + 'A';
        } else {
          result[i] = str[i];
        }
      }
    }
  }

  return result;
}

int s21_is_trim_char(char c, const char *trim_chars) {
  int result = 0;

  while (*trim_chars) {
    if (*trim_chars == c) {
      result = 1;
    }
    trim_chars++;
  }

  return result;
}

void *s21_trim(const char *src, const char *trim_chars) {
  char *result = s21_NULL;

  if (src) {
    if (!trim_chars) {
      trim_chars = " \t\n\v\f\r";
    }

    const char *start = src;
    const char *end = src + s21_strlen(src) - 1;

    while (*start && s21_is_trim_char(*start, trim_chars)) {
      start++;
    }

    if (*start != '\0') {
      while (end > start && s21_is_trim_char(*end, trim_chars)) {
        end--;
      }

      s21_size_t trimmed_length = end - start + 1;
      result = (char *)calloc(trimmed_length + 1, sizeof(char));
      if (result) {
        for (s21_size_t i = 0; i < trimmed_length; i++) {
          result[i] = start[i];
        }
        result[trimmed_length] = '\0';
      }
    } else {
      result = (char *)calloc(1, sizeof(char));
    }
  }

  return result;
}
