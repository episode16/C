#include "s21_decimal.h"
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * S21_BIG_DECIMAL.C * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
// функция для изменения знака числа децимал
void s21_big_set_sign(s21_big_decimal *num, int sign) {
  if (sign == 1) {
    num->bits[7] |= 0x80000000;  // Устанавливаем старший бит в 1
  } else {
    num->bits[7] &= ~0x80000000;  // Сбрасываем старший бит в 0
  }
}

// функция для получения знака (0 - полож, 1 - отриц)
int s21_big_get_sign(s21_big_decimal num) {
  int sign = (num.bits[7] >> 31) & 1;
  return sign;
}

// функция для получения значения степени плавающей точки
int s21_big_get_scale(s21_big_decimal num) {
  int tmp = num.bits[7] << 1;  // избавляемся от знака
  int scale = tmp >> 17;       // убираем нули от 0 до 15
  return scale;
}

// функция для изменения значения степени плавающей точки
void s21_big_set_scale(s21_big_decimal *num, int scale) {
  int sign = s21_big_get_sign(*num);
  num->bits[7] = 0;
  s21_big_set_sign(num, sign);
  num->bits[7] |= scale << 16;
}

// переписал функцию 07.08
// определяем в каком инте нужный бит
int s21_big_get_int(int bit) {
  int result;
  for (int i = 0; i < 8; i++) {
    int tmp = (32 * (i + 1)) - 1;
    if (bit >= 32 * i && bit <= tmp) result = i;
  }
  return result;
}

// функция для получения значения определенного бита в децимал
int s21_big_get_bit(s21_big_decimal num, int bit) {
  int i = s21_big_get_int(bit);
  bit = bit - (32 * i);
  int tmp = (num.bits[i] >> bit) & 1;
  return tmp;
}

// функция для изменения 1 бита в децимал
void s21_big_set_bit(s21_big_decimal *num, int bit, unsigned int value) {
  int i = s21_big_get_int(bit);
  bit = bit - (32 * i);
  if (value) num->bits[i] |= value << bit;
  if (!value) {
    value = 1;
    unsigned int tmp = ~num->bits[i];
    tmp |= value << bit;
    num->bits[i] = ~tmp;
  }
}

// функция проверки переполнения при переходе из бигдецимал в обычный
int s21_big_overflow(s21_big_decimal num) {
  int result = 0;
  for (int i = 223; i >= 96; i--) {
    if (s21_big_get_bit(num, i) != 0) result = 1;
  }
  return result;
}

// функция копирования децимал в бигдецимал (4 инта)
void s21_nebig_v_big(s21_decimal mnum, s21_big_decimal *bnum) {
  s21_big_decimal res = {0};
  res.bits[7] = mnum.bits[3];
  for (int i = 2; i >= 0; i--) {
    res.bits[i] = mnum.bits[i];
  }
  *bnum = res;
}

// функция копирования бигдецимал в децимал (4 инта)
void s21_big_v_nebig(s21_big_decimal bnum, s21_decimal *mnum) {
  s21_decimal res = {0};
  res.bits[3] = bnum.bits[7];
  for (int i = 2; i >= 0; i--) {
    res.bits[i] = bnum.bits[i];
  }
  *mnum = res;
}

// функция перевода из бигдецимал в обычный с проверкой на переполнение
int s21_big_num(s21_big_decimal *num, s21_decimal *res) {
  int scale = s21_big_get_scale(*num);
  int error = 0;
  int ostatok = 0;
  int remainder = 0;
  while (s21_big_overflow(*num) && scale && !error) {
    remainder = big_mod_ten(num);
    if (remainder) ostatok = 1;
    error = big_div_ten(num);
    scale = s21_big_get_scale(*num) - 1;
    s21_big_set_scale(num, scale);
  }
  if (((remainder == 5 && (big_mod_ten(num) % 2)) || remainder > 5 ||
       (remainder == 5 && ostatok)) &&
      !error) {
    s21_big_decimal result;
    s21_big_decimal one = {{1, 0, 0, 0, 0, 0, 0, 0}, 0};
    error = s21_big_slozh(*num, one, &result);
    if (!error) {
      s21_big_set_sign(&result, s21_big_get_sign(*num));
      s21_big_set_scale(&result, s21_big_get_scale(*num));
      *num = result;
    }
    scale = s21_big_get_scale(*num);
  }
  if (!error) s21_big_v_nebig(*num, res);
  if (s21_big_overflow(*num)) error = 1;
  return error;
}

int s21_compare_mantis(s21_big_decimal a, s21_big_decimal b) {
  int res = 0;
  int run = 1;
  for (int i = 223; i >= 0 && run; i--) {
    int bit1 = s21_big_get_bit(a, i);
    int bit2 = s21_big_get_bit(b, i);
    if (bit1 < bit2) {
      res = -1;
      run = 0;
    }
    if (bit1 > bit2) {
      res = 1;
      run = 0;
    }
  }
  return res;
}

// функция сравнения биг децимал
int s21_compare(s21_big_decimal a, s21_big_decimal b) {
  int res = 0;
  int run = 1;
  int sign1 = s21_big_get_sign(a);
  int sign2 = s21_big_get_sign(b);
  int nol = 1;
  for (int i = 223; i >= 0 && run; i--) {
    int bit1 = s21_big_get_bit(a, i);
    int bit2 = s21_big_get_bit(b, i);
    if (bit1 > 0 || bit2 > 0) nol = 0;
    if ((bit1 < bit2 && sign1 == 0) || (bit1 > bit2 && sign1 == 1)) {
      // если 1е число меньше 2го, то вернем -1
      res = -1;
      run = 0;
    }
    if ((bit1 > bit2 && sign1 == 0) || (bit1 < bit2 && sign1 == 1)) {
      // если 1е число больше 2го, то вернем 1
      res = 1;
      run = 0;
    }
  }
  if (sign1 > sign2 && (res != 0 || !nol)) {
    res = -1;
  }
  if (sign1 < sign2 && (res != 0 || !nol)) {
    res = 1;
  }
  return res;  // если ниодно условие не было выполнено, то вернется ноль
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * S21_NORMAL.C* * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

// функция для суммирования биг децимал
int s21_big_slozh(s21_big_decimal num1, s21_big_decimal num2,
                  s21_big_decimal *result) {
  s21_big_decimal res = {0};
  unsigned int v_ume = 0;
  int overflow = 0;
  for (int i = 0; i < 224; i++) {
    unsigned int num1_bit = s21_big_get_bit(num1, i);
    unsigned int num2_bit = s21_big_get_bit(num2, i);
    unsigned int result_bit = num1_bit + num2_bit + v_ume;
    v_ume = result_bit / 2;
    result_bit = result_bit % 2;
    s21_big_set_bit(&res, i, result_bit);
    if (i == 223 && v_ume) overflow++;
  }
  s21_big_set_sign(&res, s21_big_get_sign(num1));
  *result = res;
  return overflow;
}

// функция для переноса крайнего бита в следующий инт
void s21_big_perenos_bita(s21_big_decimal *num, int bit) {
  int i = s21_big_get_int(bit) + 1;
  num->bits[i] = num->bits[i] << 1;
  int kruy = s21_big_get_bit(*num, bit);
  s21_big_set_bit(num, bit + 1, kruy);
}

// функция сдвига влево бигдецимал на 1
int s21_big_sdvig_vlevo(s21_big_decimal *num) {
  int kruy = s21_big_get_bit(*num, 223);
  int res = 1;
  if (kruy == 0) {
    for (int i = 6; i > 0; i--) {
      int bit = (32 * i) - 1;
      s21_big_perenos_bita(num, bit);
    }
    num->bits[0] = num->bits[0] << 1;
    res = 0;
  }
  return res;
}

void shift_right(s21_big_decimal *value, int shift) {
  for (int i = 0; i < shift; i++) {
    unsigned int carry = 0;

    for (int j = 7; j >= 0; j--) {
      unsigned int new_carry = (value->bits[j] & 1) != 0;
      value->bits[j] = (value->bits[j] >> 1) | (carry << 31);
      carry = new_carry;
    }
  }
}

// умножаем на 10 и увеличиваем степень
int s21_na10_umnozhenie(s21_big_decimal num, s21_big_decimal *result) {
  s21_big_decimal num1 = num;
  s21_big_decimal num2 = num;
  int overflow = 0;
  for (int i = 0; i < 3 && !overflow; i++) {
    overflow = s21_big_sdvig_vlevo(&num1);
  }
  s21_big_sdvig_vlevo(&num2);
  if (s21_big_slozh(num1, num2, result)) overflow = 1;
  return overflow;
}

// функция по увеличению степени
int s21_big_scale_uvelichenie(s21_big_decimal *num) {
  s21_big_decimal result;
  int scale = s21_big_get_scale(*num);
  int overflow = s21_na10_umnozhenie(*num, &result);
  if (!overflow) {
    scale++;
    s21_big_set_scale(&result, scale);
    *num = result;
  }
  return overflow;
}

// функция зануляет биг децимал
void big_reset_decimal(s21_big_decimal *dcml) {
  for (int i = 0; i < 8; ++i) dcml->bits[i] = 0;
}

// возвращает остаток деления
int big_mod_ten(s21_big_decimal *dcml) {
  int remainder = 0;
  int i = 6, j = 31;
  while (!(dcml->bits[i] & (1 << j)) && (i >= 0)) {
    --j;
    if (j == -1) {
      --i;
      j = 31;
    }
  }
  for (; i >= 0; --i) {
    for (; j >= 0; --j) {
      remainder <<= 1;
      if (dcml->bits[i] & (1 << j)) remainder += 1;
      remainder -= remainder >= 10 ? 10 : 0;
    }
    j = 31;
  }
  return remainder;
}

// Делит число big_dcml на 10.
int big_div_ten(s21_big_decimal *dcml) {
  int error = 0;
  s21_big_decimal result;
  big_reset_decimal(&result);
  int i = 6, j = 31;
  while (!(dcml->bits[i] & (1 << j)) && (i >= 0)) {
    --j;
    if (j == -1) {
      --i;
      j = 31;
    }
  }
  if (i >= 0) {
    int remainder = 0;
    for (; i >= 0; --i) {
      for (; j >= 0; --j) {
        remainder <<= 1;
        if (dcml->bits[i] & (1 << j)) remainder += 1;
        error = s21_big_sdvig_vlevo(&result);
        if (remainder >= 10) {
          result.bits[0] |= 1;
          remainder -= 10;
        }
      }
      j = 31;
    }
    s21_big_set_sign(&result, s21_big_get_sign(*dcml));
    s21_big_set_scale(&result, s21_big_get_scale(*dcml));
    *dcml = result;
  }
  return error;
}

// Округляет последнее число в big_dcml.
int big_round_last_digit(s21_big_decimal *dcml) {
  int remainder = big_mod_ten(dcml);
  int err = big_div_ten(dcml);
  s21_big_set_scale(dcml, s21_big_get_scale(*dcml) - 1);
  if (((remainder == 5 && (big_mod_ten(dcml) % 2)) || remainder > 5) && !err) {
    s21_big_decimal result;
    s21_big_decimal one = {{1, 0, 0, 0, 0, 0, 0, 0}, 0};
    err = s21_big_slozh(*dcml, one, &result);
    if (!err) {
      s21_big_set_sign(&result, s21_big_get_sign(*dcml));
      s21_big_set_scale(&result, s21_big_get_scale(*dcml));
      *dcml = result;
    }
  }
  return err;
}

// нормализация (приведение чисел к общей степени плавающей точки)
int s21_normal(s21_big_decimal *num1, s21_big_decimal *num2) {
  int scale1 = s21_big_get_scale(*num1);
  int scale2 = s21_big_get_scale(*num2);
  int min_scale = 0;
  int max_scale = 0;
  int overflow = 0;
  s21_big_decimal num_min, num_max;
  if (scale1 < scale2) {
    min_scale = scale1;
    max_scale = scale2;
    num_min = *num1;
    num_max = *num2;
  } else {
    min_scale = scale2;
    max_scale = scale1;
    num_min = *num2;
    num_max = *num1;
  }
  while (min_scale != max_scale && min_scale <= 28 && !overflow) {
    overflow = s21_big_scale_uvelichenie(&num_min);
    min_scale++;
  }
  while (min_scale != max_scale && max_scale && !overflow) {
    overflow = big_round_last_digit(&num_max);
    max_scale--;
  }
  if (min_scale != max_scale) overflow = 1;
  if (scale1 < scale2) {
    *num1 = num_min;
    *num2 = num_max;
  } else {
    *num2 = num_min;
    *num1 = num_max;
  }

  return overflow;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * S21_OTHER.C * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
// функция для печати одного инта
// void s21_print_binary(int num) {
//   for (int i = 31; i >= 0; i--) {
//     int tmp = (num >> i) & 1;
//     printf("%d", tmp);
//   }
// }

// // функция для печати децимал
// void s21_print_decimal(s21_decimal num) {
//   for (int i = 3; i >= 0; i--) {
//     s21_print_binary(num.bits[i]);
//     printf("\n");
//   }
// }

// функция для изменения знака числа децимал
void s21_set_sign(s21_decimal *num, int sign) {
  if (sign == 1) num->bits[3] |= sign << 31;
  if (sign == 0) {
    sign = 1;
    int tmp = ~num->bits[3];
    tmp |= sign << 31;
    num->bits[3] = ~tmp;
  }
}

// функция для получения знака (0 - полож, 1 - отриц)
int s21_get_sign(s21_decimal num) {
  int sign = (num.bits[3] >> 31) & 1;
  return sign;
}

// функция для получения значения степени плавающей точки
int s21_get_scale(s21_decimal num) {
  int tmp = num.bits[3] << 1;  // избавляемся от знака
  int scale = tmp >> 17;       // убираем нули от 0 до 15
  return scale;
}

// функция для изменения значения степени плавающей точки
void s21_set_scale(s21_decimal *num, int scale) {
  int sign = s21_get_sign(*num);
  num->bits[3] = 0;
  s21_set_sign(num, sign);
  num->bits[3] |= scale << 16;
}

// переписал функцию 07.08
// определяем в каком инте нужный бит
int s21_get_int(int bit) {
  int result;
  for (int i = 0; i < 4; i++) {
    int tmp = (32 * (i + 1)) - 1;
    if (bit >= 32 * i && bit <= tmp) result = i;
  }
  return result;
}

// функция для получения значения определенного бита в децимал
int s21_get_bit(s21_decimal num, int bit) {
  int i = s21_get_int(bit);
  bit = bit - (32 * i);
  int tmp = (num.bits[i] >> bit) & 1;
  return tmp;
}

// функция для изменения 1 бита в децимал
void s21_set_bit(s21_decimal *num, int bit, unsigned int value) {
  int i = s21_get_int(bit);
  bit = bit - (32 * i);
  if (value) num->bits[i] |= value << bit;
  if (!value) {
    value = 1;
    int tmp = ~num->bits[i];
    tmp |= value << bit;
    num->bits[i] = ~tmp;
  }
}
/*
// функция подсчета битов
int get_bits_count(s21_big_decimal value) {
  for (int i = 255; i >= 0; i--) {
    if (s21_big_get_bit(value, i)) return i + 1;
  }
  return 0;
}

// функция для изменения 1 бита в децимал
void s21_e_set_bit(s21_big_decimal *value, int index) {
  int bit_index = index / 32;
  int bit_position = index % 32;
  value->bits[bit_index] |= (1 << bit_position);
}

// функция бинарного сложения
void s21_binary_add(s21_big_decimal first_value, s21_big_decimal second_value,
                    s21_big_decimal *result) {
  *result = (s21_big_decimal){{0, 0, 0, 0, 0, 0, 0, 0}, 0};
  int carry = 0;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 32; j++) {
      int first_value_bit = s21_big_get_bit(first_value, i * 32 + j);
      int second_value_bit = s21_big_get_bit(second_value, i * 32 + j);
      int bit_sum = first_value_bit + second_value_bit + carry;
      s21_big_set_bit(result, i * 32 + j, bit_sum % 2);
      carry = bit_sum / 2;
    }
  }
}

// функция бинарного вычитания
void s21_binary_sub(s21_big_decimal first_value, s21_big_decimal second_value,
                    s21_big_decimal *result) {
  *result = (s21_big_decimal){{0, 0, 0, 0, 0, 0, 0, 0}, 0};  // Инициализация
  int borrow = 0;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 32; j++) {
      int first_value_bit = s21_big_get_bit(first_value, i * 32 + j);
      int second_value_bit = s21_big_get_bit(second_value, i * 32 + j);
      int bit_diff = first_value_bit - second_value_bit - borrow;
      if (bit_diff < 0) {
        bit_diff += 2;
        borrow = 1;
      } else {
        borrow = 0;
      }
      s21_big_set_bit(result, i * 32 + j, bit_diff);
    }
  }
}

// функция бинарного умножения
void s21_binary_mul(s21_big_decimal first_value, s21_big_decimal second_value,
                    s21_big_decimal *result) {
  *result = (s21_big_decimal){{0, 0, 0, 0, 0, 0}, 0};
  s21_big_decimal result_tmp = {{0, 0, 0, 0, 0, 0}, 0};
  s21_big_decimal multiplier = first_value;
  int bit_count = get_bits_count(second_value);
  for (int i = 0; i <= bit_count; i++) {
    if (s21_big_get_bit(second_value, i)) {
      shift_left(&multiplier, i);
      s21_binary_add(result_tmp, multiplier, &result_tmp);
      multiplier = first_value;
    }
  }
  *result = result_tmp;
}

// функция бинарного деления
void s21_binary_div(s21_big_decimal value_1, s21_big_decimal value_2,
                    s21_big_decimal *result) {
  *result = (s21_big_decimal){{0, 0, 0, 0, 0, 0}, 0};
  s21_big_decimal remainder = value_1;
  s21_big_decimal temp_value_2 = value_2;
  int bit_count_value_1 = get_bits_count(value_1);
  int bit_count_value_2 = get_bits_count(temp_value_2);
  int shift = bit_count_value_1 - bit_count_value_2;
  int scale = 0;
  helper_for_binary_div(shift, &remainder, value_2, result, temp_value_2,
                        (s21_big_decimal){{0, 0, 0, 0, 0, 0}, 0});
  s21_big_decimal temp_fraction = (s21_big_decimal){{0, 0, 0, 0, 0, 0}, 0};
  while (s21_compare(remainder, (s21_big_decimal){{0, 0, 0, 0, 0, 0}, 0}) ==
             1 &&
         scale < 32) {
    scale++;
    s21_binary_mul(remainder, (s21_big_decimal){{10, 0, 0, 0, 0, 0}, 0},
                   &remainder);
    shift = get_bits_count(remainder) - get_bits_count(value_2);
    helper_for_binary_div(shift, &remainder, value_2, &temp_fraction,
                          temp_value_2,
                          (s21_big_decimal){{0, 0, 0, 0, 0, 0}, 0});
    s21_binary_mul(*result, (s21_big_decimal){{10, 0, 0, 0, 0, 0}, 0}, result);
    s21_binary_add(*result, temp_fraction, result);
  }
  result->scale = scale;
}

void helper_for_binary_div(int shift, s21_big_decimal *value_1,
                           s21_big_decimal dvalue_2, s21_big_decimal *result,
                           s21_big_decimal temp_dvalue_2,
                           s21_big_decimal temp_result) {
  *result = (s21_big_decimal){{0, 0, 0, 0, 0, 0}, 0};
  s21_big_decimal original_dvalue_2 = temp_dvalue_2;

  while (shift >= 0) {
    for (int i = 0; i < shift; i++) {
      shift_left_division(&temp_dvalue_2, shift);
    }
    if (s21_compare(temp_dvalue_2, *value_1) == 1) {
      temp_dvalue_2 = original_dvalue_2;
      shift--;
      continue;
    }
    if (s21_compare(*value_1, temp_dvalue_2) >= 0) {
      s21_binary_sub(*value_1, temp_dvalue_2, &temp_result);
      *value_1 = temp_result;
      s21_e_set_bit(result, shift);
    } else {
      int bit_index = shift / 32;
      int bit_position = shift % 32;
      result->bits[bit_index] &= ~(1 << bit_position);
    }
    shift--;
    temp_dvalue_2 = dvalue_2;
  }
}

void shift_left_division(s21_big_decimal *value, int shift) {
  while (shift--) {
    unsigned int carry = 0;
    for (int i = 0; i < 8; i++) {
      unsigned int new_carry = value->bits[i] >> 31;
      value->bits[i] = (value->bits[i] << 1) | carry;
      carry = new_carry;
    }
  }
}
*/
void shift_left(s21_big_decimal *value, int shift) {
  for (int i = 0; i < shift; i++) {
    unsigned int carry = 0;
    for (int j = 0; j < 6; j++) {
      unsigned int new_carry = (value->bits[j] & (1 << 31)) != 0;
      value->bits[j] = (value->bits[j] << 1) | carry;
      carry = new_carry;
    }
  }
}

void s21_abs(s21_big_decimal *val1, s21_big_decimal *val2) {
  s21_big_set_sign(val1, 0);
  s21_big_set_sign(val2, 0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * АРИФМЕТИКА* * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * S21_ARITHMETIC.C* * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
// функция подсчета битов
int get_bits_count(s21_big_decimal value) {
  for (int i = 223; i >= 0; i--) {
    if (s21_big_get_bit(value, i)) return i + 1;
  }
  return 0;
}

void s21_e_set_bit(s21_big_decimal *value, int index) {
  int bit_index = index / 32;
  int bit_position = index % 32;
  value->bits[bit_index] |= (1 << bit_position);
}

// функция бинарного сложения
void s21_binary_add(s21_big_decimal first_value, s21_big_decimal second_value,
                    s21_big_decimal *result) {
  *result = (s21_big_decimal){{0, 0, 0, 0, 0, 0, 0, 0}, 0};
  int carry = 0;
  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 32; j++) {
      int first_value_bit = s21_big_get_bit(first_value, i * 32 + j);
      int second_value_bit = s21_big_get_bit(second_value, i * 32 + j);
      int bit_sum = first_value_bit + second_value_bit + carry;
      s21_big_set_bit(result, i * 32 + j, bit_sum % 2);
      carry = bit_sum / 2;
    }
  }
}

// функция бинарного вычитания
void s21_binary_sub(s21_big_decimal first_value, s21_big_decimal second_value,
                    s21_big_decimal *result) {
  *result = (s21_big_decimal){{0, 0, 0, 0, 0, 0, 0, 0}, 0};  // Инициализация
  int borrow = 0;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 32; j++) {
      int first_value_bit = s21_big_get_bit(first_value, i * 32 + j);
      int second_value_bit = s21_big_get_bit(second_value, i * 32 + j);
      int bit_diff = first_value_bit - second_value_bit - borrow;
      if (bit_diff < 0) {
        bit_diff += 2;
        borrow = 1;
      } else {
        borrow = 0;
      }
      s21_big_set_bit(result, i * 32 + j, bit_diff);
    }
  }
}

// функция бинарного умножения
void s21_binary_mul(s21_big_decimal first_value, s21_big_decimal second_value,
                    s21_big_decimal *result) {
  *result = (s21_big_decimal){{0, 0, 0, 0, 0, 0, 0, 0}, 0};
  s21_big_decimal temp_result = {{0, 0, 0, 0, 0, 0, 0, 0}, 0};
  s21_big_decimal multiplicand = first_value;
  int bits = get_bits_count(second_value);

  for (int i = 0; i <= bits; i++) {
    if (s21_big_get_bit(second_value, i)) {
      // s21_big_sdvig_vlevo(&multiplicand);
      shift_left(&multiplicand, i);
      s21_binary_add(temp_result, multiplicand, &temp_result);
      multiplicand = first_value;
    }
  }
  *result = temp_result;
}

void s21_binary_div(s21_big_decimal value_1, s21_big_decimal value_2,
                    s21_big_decimal *result) {
  // Инициализация результата как 0
  *result = (s21_big_decimal){{0}, 0};

  // Остаток и временная переменная для делителя
  s21_big_decimal remainder = value_1;

  // Получаем количество битов в значениях
  int bit_count_value_1 = get_bits_count(value_1);
  int bit_count_value_2 = get_bits_count(value_2);

  // Вычисляем сдвиг для выравнивания делимого и делителя
  int shift = bit_count_value_1 - bit_count_value_2;

  int scale1 = s21_big_get_scale(value_1);
  int scale2 = s21_big_get_scale(value_2);

  // Вызов вспомогательной функции для начального деления
  helper_for_binary_div(shift, &remainder, value_2, result);

  int overflow = 0;
  int scale = scale1 - scale2;
  while (scale < 28 && !overflow &&
         (s21_compare_mantis(remainder, (s21_big_decimal){{0}, 0}) == 1)) {
    scale++;
    int overflow1 = s21_big_scale_uvelichenie(&remainder);
    int overflow2 = s21_big_scale_uvelichenie(result);
    if (overflow1 || overflow2) overflow = 1;
    shift = get_bits_count(remainder) - get_bits_count(value_2);
    s21_big_decimal tmp_res1 = {{0}, 0};
    s21_big_decimal tmp_res2 = {{0}, 0};
    helper_for_binary_div(shift, &remainder, value_2, &tmp_res1);
    s21_big_slozh(*result, tmp_res1, &tmp_res2);
    *result = tmp_res2;
    s21_big_set_scale(result, scale);
  }
  int ostatok;
  int flag = 0;
  while (s21_compare_mantis(
             remainder, (s21_big_decimal){{10, 0, 0, 0, 0, 0, 0, 0}, 0}) >= 0) {
    ostatok = big_mod_ten(&remainder);
    big_div_ten(&remainder);
    if (ostatok) flag = 1;
  }
  ostatok = remainder.bits[0];
  if ((ostatok == 5 && (big_mod_ten(&remainder) % 2)) || ostatok > 5 ||
      (ostatok == 5 && flag)) {
    s21_big_decimal one = {{1, 0, 0, 0, 0, 0, 0, 0}, 0};
    s21_big_slozh(*result, one, result);
  }
  s21_big_set_scale(result, scale);
}

// Вспомогательная функция: `helper_for_binary_div`

void helper_for_binary_div(int shift, s21_big_decimal *remainder,
                           s21_big_decimal value_2, s21_big_decimal *result) {
  // Инициализация результата как 0
  s21_big_decimal temp_result = {{0}, 0};
  *result = (s21_big_decimal){{0}, 0};
  s21_big_decimal temp_dvalue_2 = value_2;

  // Основной цикл деления с учетом сдвига
  while (shift >= 0) {
    for (int i = 0; i < shift; i++) {
      s21_big_sdvig_vlevo(&temp_dvalue_2);
      //  shift_left_division(&temp_dvalue_2, shift); // Сдвиг влево делителя
    }
    // s21_big_sdvig_vlevo(result);
#ifdef DEBUG
    printf("______helper_for_binary_div________\n");
    printf("shift= %d\n", shift);
    printf("remainder:\n");
    s21_big_print_decimal(*remainder);
    printf("temp_dvalue_2:\n");
    s21_big_print_decimal(temp_dvalue_2);
#endif

    // Если временный делитель больше остатка
    if (s21_compare_mantis(temp_dvalue_2, *remainder) == 1) {
      temp_dvalue_2 = value_2;  // Возврат к оригинальному делителю
      shift--;                  // Уменьшаем сдвиг
      // printf("!!!!!!!!!!!!temp_dvalue_2 > *value_1\n");
      continue;
    }

    // Если остаток больше или равен временному делителю
    if (s21_compare_mantis(*remainder, temp_dvalue_2) >= 0) {
      s21_binary_sub(*remainder, temp_dvalue_2,
                     &temp_result);  // Вычитаем делитель из остатка
      s21_big_set_scale(&temp_result, s21_big_get_scale(*remainder));
      *remainder = temp_result;  // Обновляем остаток
#ifdef DEBUG
      printf("*remainder:\n");
      s21_big_print_decimal(*remainder);
#endif
      s21_e_set_bit(result,
                    shift);  // Устанавливаем бит результата на позиции shift
                             // s21_big_set_bit(result, 1, 1);

#ifdef DEBUG
      printf("result>=:\n");
      s21_big_print_decimal(*result);
#endif
    } else {
      // Если остаток меньше делителя, устанавливаем бит в ноль
      int bit_index = shift / 32;
      int bit_position = shift % 32;
      result->bits[bit_index] &= ~(1 << bit_position);
#ifdef DEBUG
      printf("result<:\n");
      s21_big_print_decimal(*result);
#endif
    }
    shift--;  // Уменьшаем сдвиг
    temp_dvalue_2 =
        value_2;  // Возвращаем временный делитель к оригинальному значению
  }
  // printf("exit\n");
}

void shift_left_division(s21_big_decimal *value, int shift) {
  while (shift--) {
    unsigned int carry = 0;
    // Переменная для переноса
    for (int i = 0; i < 6; i++) {
      unsigned int new_carry =
          value->bits[i] >> 31;  // Сохраняем старший бит для переноса
      value->bits[i] =
          (value->bits[i] << 1) | carry;  // Сдвигаем влево и добавляем перенос
      carry = new_carry;  // Обновляем перенос
    }
  }
}

// void print_big_dec(s21_big_decimal num){
//   for (int i = 0; i < 8; i++)
//   {
//     printf("%d ", num.bits[i]);
//   }
//   puts("");
// }

// void print_dec2(s21_decimal num){
//   for (int i = 0; i < 4; i++)
//   {
//     printf("%d ", num.bits[i]);
//   }
//   puts("");
// }

// void s21_abs(s21_big_decimal *val1, s21_big_decimal *val2) {
//     s21_big_set_sign(val1, 0);
//     s21_big_set_sign(val2, 0);
// }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * S21_ADD.C * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
// функция сложения
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = 0;
  int sign_1 = s21_get_sign(value_1);
  int sign_2 = s21_get_sign(value_2);
  *result = (s21_decimal){{0, 0, 0, 0}};

  s21_big_decimal big_val1 = {{0, 0, 0, 0, 0, 0, 0, 0}, 0};
  s21_big_decimal big_val2 = {{0, 0, 0, 0, 0, 0, 0, 0}, 0};
  s21_big_decimal big_result = {{0, 0, 0, 0, 0, 0, 0, 0}, 0};
  s21_nebig_v_big(value_1, &big_val1);
  s21_nebig_v_big(value_2, &big_val2);
  s21_normal(&big_val1, &big_val2);
  int tmp_big_scale = s21_big_get_scale(big_val1);
  int cmp_sign =
      s21_compare(big_val1, big_val2);  // Сравниваем знаки исходных чисел
  s21_abs(&big_val1, &big_val2);  // Приводим big_val1 и big_val2 к модулю
  int cmp_val =
      s21_compare(big_val1, big_val2);  // Сравниваем значения исходных чисел
  if (cmp_sign != 0 && sign_1 != sign_2) {  // Если знаки разные
    if (cmp_val == 1) {  // Если 1-е число больше 2
      s21_binary_sub(big_val1, big_val2, &big_result);
      s21_big_set_sign(&big_result, sign_1);
    }
    if (cmp_val == -1) {  // Если 2-е число больше 1
      s21_binary_sub(big_val2, big_val1, &big_result);
      s21_big_set_sign(&big_result, sign_2);
    }
    if (cmp_val == 0) {
      *result = (s21_decimal){{0, 0, 0, 0}};
    }
  } else {
    s21_binary_add(big_val1, big_val2, &big_result);
    s21_big_set_sign(&big_result, sign_1);
  }
  int tmp_sign = s21_big_get_sign(big_result);
  s21_big_set_scale(&big_result, tmp_big_scale);
  status = s21_big_num(&big_result, result);
  if (status == 1 && tmp_sign == 1) {
    status = 2;
  } else if (status == 1 && tmp_sign == 0) {
    status = 1;
  }
  return status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * S21_SUB.C * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
// функция вычитания
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = 0;
  int sign_1 = s21_get_sign(value_1);
  int sign_2 = s21_get_sign(value_2);
  *result = (s21_decimal){{0, 0, 0, 0}};

  s21_big_decimal big_val1 = {{0, 0, 0, 0, 0, 0, 0, 0}, 0};
  s21_big_decimal big_val2 = {{0, 0, 0, 0, 0, 0, 0, 0}, 0};
  s21_big_decimal big_result = {{0, 0, 0, 0, 0, 0, 0, 0}, 0};
  s21_nebig_v_big(value_1, &big_val1);
  s21_nebig_v_big(value_2, &big_val2);
  s21_normal(&big_val1, &big_val2);
  int tmp_big_scale = s21_big_get_scale(big_val1);
  int cmp_sign =
      s21_compare(big_val1, big_val2);  // Сравниваем знаки исходных чисел
  s21_abs(&big_val1, &big_val2);  // Приводим big_val1 и big_val2 к модулю
  int cmp_val =
      s21_compare(big_val1, big_val2);  // Сравниваем значения исходных чисел
  if (sign_1 == sign_2 && sign_1 == 0) {  // Если знаки одинаковые положительные
    if (cmp_val == 1) {  // Если 1-е число больше 2
      s21_binary_sub(big_val1, big_val2, &big_result);
      s21_big_set_sign(&big_result, sign_1);
    }
    if (cmp_val == -1) {  // Если 2-е число больше 1
      s21_binary_sub(big_val2, big_val1, &big_result);
      s21_big_set_sign(&big_result, 1);
    }
    if (cmp_val == 0) {
      *result = (s21_decimal){{0, 0, 0, 0}};
    }
  } else if (sign_1 == sign_2 &&
             sign_1 == 1) {  // Если знаки одинаковые отрицательные
    if (cmp_val == 1) {  // Если 1-е число больше 2
      s21_binary_sub(big_val1, big_val2, &big_result);
      s21_big_set_sign(&big_result, sign_1);
    }
    if (cmp_val == -1) {  // Если 2-е число больше 1
      s21_binary_sub(big_val2, big_val1, &big_result);
      s21_big_set_sign(&big_result, 0);
    }
  } else if (cmp_sign == 1 && sign_1 == 0 && sign_2 == 1) {
    s21_binary_add(big_val1, big_val2, &big_result);
    s21_big_set_sign(&big_result, sign_1);
  } else if (cmp_sign == -1 && sign_1 == 1 && sign_2 == 0) {
    s21_binary_add(big_val1, big_val2, &big_result);
    s21_big_set_sign(&big_result, sign_1);
  }
  int tmp_sign = s21_big_get_sign(big_result);
  s21_big_set_scale(&big_result, tmp_big_scale);
  status = s21_big_num(&big_result, result);
  if (status == 1 && tmp_sign == 1) {
    status = 2;
  } else if (status == 1 && tmp_sign == 0) {
    status = 1;
  }
  return status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * S21_MUL.C * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
// функция умножения
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = 0;
  int sign_1 = s21_get_sign(value_1);
  int sign_2 = s21_get_sign(value_2);
  *result = (s21_decimal){{0, 0, 0, 0}};
  int scale1 = s21_get_scale(value_1);
  int scale2 = s21_get_scale(value_2);
  int scale = scale1 + scale2;
  s21_big_decimal big_val1 = {{0, 0, 0, 0, 0, 0, 0, 0}, 0};
  s21_big_decimal big_val2 = {{0, 0, 0, 0, 0, 0, 0, 0}, 0};
  s21_big_decimal big_result = {{0, 0, 0, 0, 0, 0, 0, 0}, 0};

  s21_nebig_v_big(value_1, &big_val1);
  s21_nebig_v_big(value_2, &big_val2);
  s21_binary_mul(big_val1, big_val2, &big_result);
  s21_big_set_scale(&big_result, scale);
  status = s21_big_num(&big_result, result);
  if (sign_1 != sign_2)
    s21_set_sign(result, 1);
  else
    s21_set_sign(result, 0);
  int tmp_sign = s21_get_sign(*result);
  if (status == 1 && tmp_sign == 1) {
    *result = (s21_decimal){{0, 0, 0, 0}};
    status = 2;
  } else if (status == 1 && tmp_sign == 0) {
    *result = (s21_decimal){{0, 0, 0, 0}};
    status = 1;
  }
  return status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * S21_DIV.C * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (!result) return 0;
  int status = 0;
  int sign_1 = s21_get_sign(value_1);
  int sign_2 = s21_get_sign(value_2);
  *result = (s21_decimal){{0, 0, 0, 0}};

  s21_big_decimal big_val1 = {{0, 0, 0, 0, 0, 0, 0, 0}, 0};
  s21_big_decimal big_val2 = {{0, 0, 0, 0, 0, 0, 0, 0}, 0};
  s21_big_decimal big_result = {{0, 0, 0, 0, 0, 0, 0, 0}, 0};

  s21_nebig_v_big(value_1, &big_val1);
  s21_nebig_v_big(value_2, &big_val2);

  s21_normal(&big_val1, &big_val2);
#ifdef DEBUG
  printf("big_val1=\n");
  s21_big_print_decimal(big_val1);
  printf("big_val2=\n");
  s21_big_print_decimal(big_val2);
#endif

  if (s21_compare_mantis(big_val2,
                         (s21_big_decimal){{0, 0, 0, 0, 0, 0, 0}, 0}) == 0)
    status = 3;
  else {
    s21_binary_div(big_val1, big_val2, &big_result);

    status = s21_big_num(&big_result, result);
    if (sign_1 != sign_2)
      s21_set_sign(result, 1);
    else
      s21_set_sign(result, 0);

    int tmp_sign = s21_get_sign(*result);
    if (status == 1 && tmp_sign == 1) {
      *result = (s21_decimal){{0, 0, 0, 0}};
      status = 2;
    } else if (status == 1 && tmp_sign == 0) {
      *result = (s21_decimal){{0, 0, 0, 0}};
      status = 1;
    }
  }
  return status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * СРАВНЕНИЕ * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * S21_IS_EQUAL.C* * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
// функция проверки на равенство (0-ложь, 1-правда)
int s21_is_equal(s21_decimal chislo1, s21_decimal chislo2) {
  s21_big_decimal num1 = {0};
  s21_big_decimal num2 = {0};
  s21_nebig_v_big(chislo1, &num1);
  s21_nebig_v_big(chislo2, &num2);
  s21_normal(&num1, &num2);
  int result = 0;
  if (s21_compare(num1, num2) == 0) result = 1;
  return result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * S21_IS_GREATER.C * * * * * * * * * * * * * * * * *
 * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
// функция сравнения больше (0-ложь, 1-правда)
int s21_is_greater(s21_decimal chislo1, s21_decimal chislo2) {
  s21_big_decimal num1 = {0};
  s21_big_decimal num2 = {0};
  s21_nebig_v_big(chislo1, &num1);
  s21_nebig_v_big(chislo2, &num2);
  s21_normal(&num1, &num2);
  int result = 0;
  if (s21_compare(num1, num2) > 0) result = 1;
  return result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * S21_IS_GREATER_OR_EQUAL.C* * * * * * * * * * * * *
 * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
// функция сравнения больше-равно (0-ложь, 1-правда)
int s21_is_greater_or_equal(s21_decimal chislo1, s21_decimal chislo2) {
  s21_big_decimal num1 = {0};
  s21_big_decimal num2 = {0};
  s21_nebig_v_big(chislo1, &num1);
  s21_nebig_v_big(chislo2, &num2);
  s21_normal(&num1, &num2);
  int result = 1;
  if (s21_compare(num1, num2) < 0) result = 0;
  return result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * S21_IS_LESS.C* * * * * * * * * * * * * * * * * * *
 * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
// функция сравнения меньше (0-ложь, 1-правда)
int s21_is_less(s21_decimal chislo1, s21_decimal chislo2) {
  s21_big_decimal num1 = {0};
  s21_big_decimal num2 = {0};
  s21_nebig_v_big(chislo1, &num1);
  s21_nebig_v_big(chislo2, &num2);
  s21_normal(&num1, &num2);
  int result = 0;
  if (s21_compare(num1, num2) < 0) result = 1;
  return result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * S21_IS_LESS_OR_EQUAL.C * * * * * * * * * * * * * *
 * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
// функция сравнения меньше-равно (0-ложь, 1-правда)
int s21_is_less_or_equal(s21_decimal chislo1, s21_decimal chislo2) {
  s21_big_decimal num1 = {0};
  s21_big_decimal num2 = {0};
  s21_nebig_v_big(chislo1, &num1);
  s21_nebig_v_big(chislo2, &num2);
  s21_normal(&num1, &num2);
  int result = 1;
  if (s21_compare(num1, num2) > 0) result = 0;
  return result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * S21_IS_NOT_EQUAL.C * * * * * * * * * * * * * * * *
 * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
// функция сравнения неравно (0-ложь, 1-правда)
int s21_is_not_equal(s21_decimal chislo1, s21_decimal chislo2) {
  s21_big_decimal num1 = {0};
  s21_big_decimal num2 = {0};
  s21_nebig_v_big(chislo1, &num1);
  s21_nebig_v_big(chislo2, &num2);
  s21_normal(&num1, &num2);
  int result = 0;
  if (s21_compare(num1, num2) != 0) result = 1;
  return result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * CONVERTATION* * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * S21_FROM_INT_TO_DECIMAL.C * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int tmp = CONVERTATION_ERROR;
  if (dst == NULL) {
    tmp = CONVERTATION_ERROR;
  } else {
    dst->bits[0] = 0;
    dst->bits[1] = 0;
    dst->bits[2] = 0;
    dst->bits[3] = 0;
    if (src <= 2147483647 && src >= -2147483647) {
      if (src < 0) {
        s21_set_sign(dst, 1);
        dst->bits[0] = (unsigned int)(-src);
      } else {
        dst->bits[0] = (unsigned int)src;
      }
      tmp = 0;
    }
  }

  return tmp;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * S21_FROM_FLOAT_TO_DECIMAL.C * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int flag = OK;
  if (!dst) {
    flag = CONVERTATION_ERROR;
    return flag;
  } else if (fabs(src) < 1e-28f || fabs(src) > 1e+28f) {
    flag = CONVERTATION_ERROR;
  } else {
    *dst = (s21_decimal){{0, 0, 0, 0}};
    char buffer[32] = {0};

    size_t ind = 0;
    for (size_t i = 0; i < strlen(buffer); i++) {
      if (buffer[i] == '.' || buffer[i] == '-' || buffer[i] == '+') {
        continue;
      }
      dst->bits[ind++] = buffer[i] - '0';
      if (ind >= 4) {
        break;
      }
    }

    if (src < 0) {
      dst->bits[3] |= 1 << 31;
    } else {
      dst->bits[3] &= ~(1 << 31);
    }
  }
  return flag;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * S21_FROM_DECIMAL_TO_INT.C * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
// src decimal
// *dst указатель на int
int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int tmp = OK;

  if (!dst) {
    tmp = CONVERTATION_ERROR;
  } else {
    s21_decimal trunc;
    s21_truncate(src, &trunc);
    int scale = s21_get_scale(src);
    if (scale < 0) scale = -scale;
    while (scale > 28) {
      if (trunc.bits[0] == 0 && scale >= 29) {
        tmp = CONVERTATION_ERROR;
        break;
      }
      trunc.bits[0] /= 10;
      scale--;
    }

    if (trunc.bits[0] > 0 && trunc.bits[0] < 4294967295 && scale <= 28) {
      *dst = trunc.bits[0];
      tmp = OK;

    } else {
      tmp = CONVERTATION_ERROR;
    }
    if (s21_get_sign(src)) {
      *dst *= -1;
    }
  }

  return tmp;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * S21_FROM_DECIMAL_TO_FLOAT.C * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
int s21_get_zero(s21_decimal decimal) {
  int res = 0;
  for (int i = 0; i < 3; i++) {
    if (decimal.bits[i] != 0) {
      res = 1;
    }
  }
  return res;
}

// src decimal конвертируемый
// dst указатель на флоат - резултат конв
// > 7.9228162514264337593543950335 or infinity
// < 1e-28

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int flag = OK;

  if (!dst) {
    flag = CONVERTATION_ERROR;
  } else {
    if (s21_get_zero(src) == 0) {
      *dst = 0.0f;
    } else {
      int scale = s21_get_scale(src);
      int sign = s21_get_sign(src);
      double tmp = 0.0;
      for (int i = 2; i >= 0; i--) {
        tmp = tmp * S21_UMAX_INT + src.bits[i];
      }
      while (scale > 0) {
        tmp /= 10.0;
        scale--;
      }
      *dst = (float)tmp;

      if (sign) {
        *dst = -*dst;
      }
      // fabs вычисляет абсолютное значение (модуль) и возвращает его
      if (fabs(*dst) < 1e-28 ||
          fabs(*dst) > 7.9228162514264337593543950335e28 ||
          fabs(*dst) == INFINITY) {
        *dst = 0.0f;
        flag = CONVERTATION_ERROR;
      }
    }
  }
  return flag;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * *HELPERS FOR OPERATIONS * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
void reset_decimal(s21_decimal *dcml) {
  for (int i = 0; i < 4; ++i) dcml->bits[i] = 0;
}

void right_shift(s21_decimal *dcml, int count) {
  while (count > 31) {
    right_shift(dcml, 31);
    count -= 31;
  }
  dcml->bits[0] >>= count;
  for (int i = 0; i < count; ++i) dcml->bits[0] &= (~(1 << (32 - 1 - i)));
  for (int i = 1; i < 4 - 1; ++i) {
    for (int j = 0; j < count; ++j)
      if (dcml->bits[i] & (1 << j))
        dcml->bits[i - 1] |= (1 << (32 - count + j));
    dcml->bits[i] >>= count;
    for (int j = 0; j < count; ++j) dcml->bits[i] &= (~(1 << (32 - 1 - j)));
  }
}

void left_shift(s21_decimal *dcml, int count) {
  while (count > 31) {
    left_shift(dcml, 31);
    count -= 31;
  }
  dcml->bits[2] <<= count;
  for (int i = 1; i >= 0; --i) {
    for (int j = 0; j < count; ++j)
      if (dcml->bits[i] & (1 << (32 - j - 1)))
        dcml->bits[i + 1] |= (1 << (count - 1 - j));
    dcml->bits[i] <<= count;
  }
}

void shift(s21_decimal *dcml, int count) {
  count >= 0 ? right_shift(dcml, count) : left_shift(dcml, -count);
}

void div_ten(s21_decimal *dcml) {
  s21_decimal result;
  reset_decimal(&result);
  int i = 2, j = 31;
  while (!(dcml->bits[i] & (1 << j)) && (i >= 0)) {
    --j;
    if (j == -1) {
      --i;
      j = 31;
    }
  }

  if (i >= 0) {
    int remainder = 0;
    for (; i >= 0; --i) {
      for (; j >= 0; --j) {
        remainder <<= 1;

        if (dcml->bits[i] & (1 << j)) remainder += 1;
        shift(&result, -1);
        if (remainder >= 10) {
          result.bits[0] |= 1;
          remainder -= 10;
        }
      }
      j = 31;
    }

    s21_set_sign(&result, s21_get_sign(*dcml));
    s21_set_scale(&result, s21_get_scale(*dcml));
    *dcml = result;
  }
}

int mod_ten(s21_decimal *dcml) {
  int remainder = 0;
  int i = 2, j = 31;
  while (!(dcml->bits[i] & (1 << j)) && (i >= 0)) {
    --j;
    if (j == -1) {
      --i;
      j = 31;
    }
  }
  for (; i >= 0; --i) {
    for (; j >= 0; --j) {
      remainder <<= 1;
      if (dcml->bits[i] & (1 << j)) remainder += 1;
      remainder -= remainder >= 10 ? 10 : 0;
    }
    j = 31;
  }
  return remainder;
}

int add_decimal_values(s21_decimal val1, s21_decimal val2,
                       s21_decimal *result) {
  reset_decimal(result);
  int overflow = 0;
  for (int i = 0; i < 4 - 1; ++i)
    for (int j = 0; j < 32; ++j)
      if (val1.bits[i] & (1 << j) && val2.bits[i] & (1 << j)) {
        if (overflow) result->bits[i] |= (1 << j);
        overflow = 1;
      } else if (val1.bits[i] & (1 << j) || val2.bits[i] & (1 << j)) {
        if (!overflow) result->bits[i] |= (1 << j);
      } else if (overflow) {
        result->bits[i] |= (1 << j);
        overflow = 0;
      }
  return overflow;
}

void round_last_digit(s21_decimal *dcml) {
  int remainder = mod_ten(dcml);
  div_ten(dcml);
  s21_set_scale(dcml, s21_get_scale(*dcml) - 1);
  if ((remainder == 5 && (mod_ten(dcml) % 2)) || remainder > 5) {
    s21_decimal result;
    s21_decimal one;
    s21_from_int_to_decimal(1, &one);  ///  ожидаю функции Карины
    int err = add_decimal_values(*dcml, one, &result);
    if (!err) {
      s21_set_sign(&result, s21_get_sign(*dcml));
      s21_set_scale(&result, s21_get_scale(*dcml));
      *dcml = result;
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * OPERATIONS* * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * S21_FLOOR.C * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

int s21_floor(s21_decimal value, s21_decimal *result) {
  int flag = 0;
  if (!result)
    flag = 1;
  else {
    *result = (s21_decimal){{0, 0, 0, 0}};
    int sign = s21_get_sign(value);
    int scale = s21_get_scale(value);
    s21_decimal temp_result = {{0, 0, 0, 0}};
    s21_truncate(value, &temp_result);

    temp_result.bits[3] = 0;
    if (sign == 1 && scale > 0) {
      s21_add(temp_result, (s21_decimal){{1, 0, 0, 0}}, &temp_result);
    }
    *result = temp_result;
    s21_set_sign(result, sign);
  }

  return flag;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * S21_ROUND.C * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
int s21_round(s21_decimal value, s21_decimal *result) {
  if (!result || s21_get_scale(value) < 0) return 1;  // 1 для ошибки

  *result = value;
  int exp = s21_get_scale(value);
  for (int i = 0; i < exp; i++) {
    round_last_digit(result);
  }

  s21_set_scale(result, 0);
  return 0;  // 0 для успешного выполнения
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * S21_TRUNCATE.C* * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int flag = 0;
  int exp = s21_get_scale(value);
  if (exp >= 0 && result) {
    *result = value;
    for (int i = 0; i < exp; i++) {
      div_ten(result);
    }
    s21_set_scale(result, 0);

  } else
    flag = 1;
  return flag;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * S21_NEGATE.C* * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
int s21_negate(s21_decimal value, s21_decimal *result) {
  if (result == NULL) {
    return 1;  // Вернуть ошибку, если указатель NULL
  }
  *result = value;
  s21_set_sign(result, !s21_get_sign(value));
  return 0;  // Успех
}

// int main(){
//   s21_decimal a = {1, 0, 0 , 0};
//   s21_decimal b = {2, 0, 0, 0};
//   s21_decimal res;
//   int i = 0;
//   float j = 0.0;
//   s21_add(a, b, &res);
//   s21_sub(a, b, &res);
//   s21_mul(a, b, &res);
//   s21_div(a, b, &res);
//   s21_is_equal(a, b);
//   s21_is_greater(a, b);
//   s21_is_less(a, b);
//   s21_is_greater_or_equal(a, b);
//   s21_is_less_or_equal(a, b);
//   s21_is_not_equal(a, b);
//   s21_from_int_to_decimal(1, &res);
//   s21_from_float_to_decimal(1.0, &res);
//   s21_from_decimal_to_int(a, &i);
//   s21_from_decimal_to_float(a, &j);
//   s21_floor(a, &res);
//   s21_round(a, &res);
//   s21_truncate(a, &res);
//   s21_negate(a, &res);
//   return 0;
// }