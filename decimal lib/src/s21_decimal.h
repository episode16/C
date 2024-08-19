#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_
#include <math.h>
#include <string.h>

#define OK 0
#define CONVERTATION_ERROR 1
#define CALCULATION_ERROR 1
#define S21_UMAX_INT 0xFFFFFFFF

// #include <stdio.h>  // удалить перед сдачей
// #define DEBUG

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned int bits[8];
  int scale;
} s21_big_decimal;

// для разработки
// void s21_print_binary(int num);
// void s21_print_decimal(s21_decimal num);
// void s21_big_print_binary(int num);
// void s21_big_print_decimal(s21_big_decimal num);

// вспомогательные
void s21_set_sign(s21_decimal *num, int sign);
int s21_get_sign(s21_decimal num);
int s21_get_scale(s21_decimal num);
void s21_set_scale(s21_decimal *num, int scale);
int s21_get_bit(s21_decimal num, int bit);
void s21_set_bit(s21_decimal *num, int bit, unsigned int value);
int s21_get_int(int bit);
void s21_print_decimal(s21_decimal num);
void s21_print_binary(int num);
void s21_big_print_decimal(s21_big_decimal num);
void s21_big_print_binary(int num);

int s21_get_zero(s21_decimal decimal);

void reset_decimal(s21_decimal *dcml);
void right_shift(s21_decimal *dcml, int count);
void left_shift(s21_decimal *dcml, int count);
void shift(s21_decimal *dcml, int count);
void div_ten(s21_decimal *dcml);
int mod_ten(s21_decimal *dcml);
int add_decimal_values(s21_decimal val1, s21_decimal val2, s21_decimal *result);
void round_last_digit(s21_decimal *dcml);

int get_bits_count(s21_big_decimal value);
void s21_e_set_bit(s21_big_decimal *value, int index);
void s21_binary_add(s21_big_decimal first_value, s21_big_decimal second_value,
                    s21_big_decimal *result);
void s21_binary_sub(s21_big_decimal first_value, s21_big_decimal second_value,
                    s21_big_decimal *result);
void s21_binary_mul(s21_big_decimal first_value, s21_big_decimal second_value,
                    s21_big_decimal *result);
void s21_binary_div(s21_big_decimal value_1, s21_big_decimal value_2,
                    s21_big_decimal *result);
void helper_for_binary_div(int shift, s21_big_decimal *value_1,
                           s21_big_decimal value_2, s21_big_decimal *result);
void shift_left_division(s21_big_decimal *value, int shift);
void shift_left(s21_big_decimal *value, int shift);
void s21_abs(s21_big_decimal *val1, s21_big_decimal *val2);
void s21_e_set_bit(s21_big_decimal *value, int index);
int s21_compare_mantis(s21_big_decimal a, s21_big_decimal b);

// биг децимал
void s21_big_set_sign(s21_big_decimal *num, int sign);
int s21_big_get_sign(s21_big_decimal num);
int s21_big_get_scale(s21_big_decimal num);
void s21_big_set_scale(s21_big_decimal *num, int scale);
int s21_big_get_bit(s21_big_decimal num, int bit);
void s21_big_set_bit(s21_big_decimal *num, int bit, unsigned int value);
int s21_big_get_int(int bit);
int s21_big_overflow(s21_big_decimal num);
void s21_nebig_v_big(s21_decimal mnum, s21_big_decimal *bnum);
void s21_big_v_nebig(s21_big_decimal bnum, s21_decimal *mnum);
int s21_big_num(s21_big_decimal *num, s21_decimal *res);
int s21_compare(s21_big_decimal a, s21_big_decimal b);

// нормализация
int s21_normal(s21_big_decimal *num1, s21_big_decimal *num2);
int s21_big_scale_uvelichenie(s21_big_decimal *num);
int s21_na10_umnozhenie(s21_big_decimal num, s21_big_decimal *result);
int s21_big_sdvig_vlevo(s21_big_decimal *num);
void s21_big_perenos_bita(s21_big_decimal *num, int bit);
int s21_big_slozh(s21_big_decimal num1, s21_big_decimal num2,
                  s21_big_decimal *result);
void big_reset_decimal(s21_big_decimal *dcml);
int big_mod_ten(s21_big_decimal *dcml);
int big_div_ten(s21_big_decimal *dcml);
int big_round_last_digit(s21_big_decimal *dcml);

// сравнение
int s21_is_not_equal(s21_decimal chislo1, s21_decimal chislo2);
int s21_is_less(s21_decimal chislo1, s21_decimal chislo2);
int s21_is_less_or_equal(s21_decimal chislo1, s21_decimal chislo2);
int s21_is_greater(s21_decimal chislo1, s21_decimal chislo2);
int s21_is_greater_or_equal(s21_decimal chislo1, s21_decimal chislo2);
int s21_is_equal(s21_decimal chislo1, s21_decimal chislo2);

// арифметика
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// преобразование
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// другие функции
int s21_round(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_floor(s21_decimal value, s21_decimal *result);

#endif
