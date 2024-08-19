#include <stdio.h>

int main() {
    // Переменные для размеров поля + положение и направление движения ракеток
    int height = 25, width = 80, up = 0, down = 25, up_2 = 0, down_2 = 25, x, y, xdir = 1, ydir = 1,
        score_1 = 0, score_2 = 0, check_score_1 = 0, check_score_2 = 0, k, l;
    x = height / 2 - 1;
    y = width / 2 - 1;
    // Основной цикл: продолжается до ввода символа '0'

    while ((score_2 != 21) && (score_1 != 21)) {
        // Получение символа
        char a;
        a = getchar();
        // Является ли символ одним из 'a', 'z', 'k', 'm'
        // Изменение положения первой ракетки при нажатии 'a' или 'z'
        if (a == 'a' || a == 'A') {
            down = down - 1;
            up = up + 1;
        }
        if (a == 'z' || a == 'Z') {
            down = down + 1;
            up = up - 1;
        }

        // Изменение положения второй ракетки при нажатии 'k' или 'm'
        if (a == 'k' || a == 'K') {
            down_2 = down_2 - 1;
            up_2 = up_2 + 1;
        }
        if (a == 'm' || a == 'M') {
            down_2 = down_2 + 1;
            up_2 = up_2 - 1;
        }

        // Отрисовка поля и ракеток при нажатии пробела

        if (a == ' ' || a == 'a' || a == 'A' || a == 'z' || a == 'Z' || a == 'k' || a == 'K' || a == 'm' ||
            a == 'M') {
            x += xdir;
            y += ydir;
            k = (down - up) / 2;
            l = (down_2 - up_2) / 2;
            if (x == 1 || x == height) {
                xdir = -xdir;
            }
            if (y == width - 1 && (x <= l + 1 - 2 || x >= l + 1)) {
                score_1 += 1;
            }
            if (y == 1 && (x <= k + 1 - 2 || x >= k + 1)) {
                score_2 += 1;
            }
            if ((y == 2 && (x == k - 1 || x == k + 1 || x == k)) ||
                (y == width - 2 && (x == l - 1 || x == l || x == l + 1))) {
                ydir = -ydir;
            }
            for (int i = 0; i <= height + 1; i++) {
                for (int j = 0; j <= width + 1; j++) {
                    // Проверка условий для отрисовки границ поля и пустого пространства
                    if ((i != 0 && j != 0) && (i != height + 1 && j != width + 1)) {
                        // Определение позиции для отрисовки ракеток
                        if (((i == ((down - up) + 1) / 2 || i == (((down - up) + 1) / 2) - 1 ||
                              i == (((down - up) + 1) / 2) + 1) &&
                             j == 1) ||
                            ((i == ((down_2 - up_2) + 1) / 2 || i == (((down_2 - up_2) + 1) / 2) - 1 ||
                              i == (((down_2 - up_2) + 1) / 2) + 1) &&
                             j == width)) {
                            // Проверка и корректировка положения ракеток в пределах допустимых значений
                            if (((down >= 25 && down <= 36) && (down_2 >= 25 && down_2 <= 36)) ||
                                ((up >= -11 && up <= 11) && (up_2 >= -11 && up_2 <= 11))) {
                                printf("|");
                            } else if (down > 36 && up < -11) {
                                down = down - 1;
                                up = up + 1;
                                printf("|");
                            } else if (down_2 > 36 && up_2 < -11) {
                                down_2 = down_2 - 1;
                                up_2 = up_2 + 1;
                                printf("|");
                            } else if (down < 14 && up > 11) {
                                down = down + 1;
                                up = up - 1;
                                printf("|");
                            } else if (down_2 < 14 && up_2 > 11) {
                                down_2 = down_2 + 1;
                                up_2 = up_2 - 1;
                                printf("|");
                            }

                        } else if (i == x && j == y) {
                            printf("*");
                        }

                        else {
                            // Отрисовка пустого пространства
                            printf(" ");
                        }
                    } else {
                        // Отрисовка границ поля
                        printf("#");
                    }
                }
                printf("\n");
            }
            printf("\n");
            printf("PLAYER 1: %d\n\nPLAYER 2: %d\n", score_1, score_2);
        }
        if (check_score_1 != score_1 || check_score_2 != score_2) {
            x = height / 2 - 1;
            y = width / 2 - 1;
            up = 0;
            down = 25;
            up_2 = 0;
            down_2 = 25;

            check_score_1 = score_1;
            check_score_2 = score_2;
        }
    }
    if (score_1 == 21) {
        printf("ПОБЕДИТЕЛЬ PLAYER 1\n");
    }
    if (score_2 == 21) {
        printf("ПОБЕДИТЕЛЬ PLAYER 2\n");
    }
    return 0;
}
