#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
//#include  <locale.h>

int main(void) {
    //setlocale(LC_ALL,"ru_RU");
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    int x,y,z;
    printf_s("Програма по выполнению генерации последовательности четных чисел \
в заданном интервале\n");
    do {
        printf_s("Введите интервал (например:'X-X' 'X,X' 'X X'):");
        while (scanf_s("%d%*c%d", &x, &y) != 2) {
            printf_s("Это не числа. Попробуй еще раз:");
            rewind(stdin);
        }
        for (int i = x; i <= y; i++) {
            if (i % 2 == 0) {
                printf_s("%d ", i);
            }
        }
        printf("\nХотите продолжить? (1 — Да, 0 — Выход): ");
        scanf_s("%d", &z);
    } while (z == 1);
    return 0;
}
