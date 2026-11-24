//
// Created by Qewa on 17.11.2026.
//
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void matrix_list() {
    int n;
    printf("Введите размер квадратной матрицы N: ");
    scanf("%d", &n);
    rewind(stdin);

    // Выделение памяти под двумерный динамический массив на Си
    int **matrix = (int **) malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        matrix[i] = (int *) malloc(n * sizeof(int));
    }

    // Ввод элементов матрицы
    printf("Введите элементы матрицы %dx%d:\n", n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }

    // Массив для сумм диагоналей (всего 2*N - 1 диагоналей)
    int num_diagonals = 2 * n - 1;
    int *sums = (int *) calloc(num_diagonals, sizeof(int)); // calloc сразу зануляет память

    // Расчет сумм: индекс сумм k = i + j
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            sums[i + j] += matrix[i][j];
        }
    }

    // Вывод результатов
    printf("\nСуммы диагоналей, параллельных побочной:\n");
    for (int k = 0; k < num_diagonals; k++) {
        printf("Диагональ k=%d (i+j=%d): сумма = %d\n", k, k, sums[k]);
    }

    // Освобождение памяти
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(sums);
}

void txt_list() {
    int letter_count[256] = {0};
    short int total_letter = 0;
    int ch;
    rewind(stdin);

    while ((ch = getchar()) != '\n') {
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= 192 && ch <= 223) || (
                ch >= 224 && ch <= 255)) {
            letter_count[ch]++;
            total_letter++;
        }
    }
    printf_s("\nРезультаты сканирования:\n>");
    printf_s("%d - Всего символов алфавита в тексте", total_letter);
    for (int i = 0; i < 256; i++) {
        if (letter_count[i] != 0) {
            printf_s("\n>'%c' - Появился %d раз", i, letter_count[i]);
        }
    }
    printf_s("\nОтчет окончен.");
}


int main(void) {
    int menu;
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    do {
        printf_s("Выбор режима программы с массивами: \n1. С числами.\n2. С текстом\n0. Выход\n");
        printf_s("> ");
        scanf_s("%d", &menu);
        switch (menu) {
            case 1:
                matrix_list();
                break;
            case 2:
                txt_list();
                break;
            default:
                printf_s("Неверный пункт! Попробуйте снова.\n");
        }
    } while (menu != 0);
    return 0;
}
