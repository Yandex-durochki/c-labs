#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// 1. Создаем нашу структуру данных
struct Person {
    char surname[50];  // Фамилия
    int day;           // День рождения
    int month;         // Месяц рождения
    int year;          // Год рождения
    char zodiac[30];   // Знак Зодиака
};

// Имя файла, в котором будет храниться наша база данных
const char* filename = "database.bin";

// --- ФУНКЦИЯ 1: ДОБАВЛЕНИЕ ЧЕЛОВЕКА В ФАЙЛ ---
void add_person() {
    struct Person p;
    printf("\nВведите фамилию: ");
    scanf("%49s", p.surname);
    printf("Введите дату рождения (День Месяц Год через пробел): ");
    if (scanf("%d %d %d", &p.day, &p.month, &p.year) != 3) {
        rewind(stdin);
        printf("Ошибка ввода даты!\n");
        return;
    }
    printf("Введите знак Зодиака: ");
    scanf("%29s", p.zodiac);
    rewind(stdin);

    // Открываем файл для ДОЗАПИСИ в бинарном режиме ("ab" - append binary)
    FILE* f = fopen(filename, "ab");
    if (f == NULL) {
        printf("Ошибка открытия файла!\n");
        return;
    }

    // Записываем 1 структуру p в файл f
    fwrite(&p, sizeof(struct Person), 1, f);
    fclose(f);
    printf("Данные успешно сохранены!\n");
}

// --- ФУНКЦИЯ 2: ВЫВОД ВСЕХ ЗАПИСЕЙ ---
void show_all() {
    // Открываем файл для ЧТЕНИЯ в бинарном режиме ("rb" - read binary)
    FILE* f = fopen(filename, "rb");
    if (f == NULL) {
        printf("Файл пуст или еще не создан.\n");
        return;
    }

    struct Person p;
    printf("\n--- Список всех людей ---\n");
    // fread читает по 1 структуре, пока файл не закончится
    while (fread(&p, sizeof(struct Person), 1, f)) {
        printf("Фамилия: %-10s | Дата: %02d.%02d.%04d | Зодиак: %s\n",
               p.surname, p.day, p.month, p.year, p.zodiac);
    }
    fclose(f);
}

// --- ФУНКЦИЯ 3: ПОИСК ПО ЗНАКУ ЗОДИАКА ---
void search_zodiac() {
    FILE* f = fopen(filename, "rb");
    if (f == NULL) {
        printf("Файл пуст или еще не создан.\n");
        return;
    }

    char search[30];
    printf("\nВведите знак Зодиака для поиска: ");
    scanf("%29s", search);
    rewind(stdin);

    struct Person p;
    int found = 0; // Флаг, нашли ли мы кого-то

    printf("\n--- Результаты поиска ---\n");
    while (fread(&p, sizeof(struct Person), 1, f)) {
        // Сравниваем строки. Если strcmp возвращает 0, строки одинаковые
        if (strcmp(p.zodiac, search) == 0) {
            printf("Фамилия: %-10s | Дата: %02d.%02d.%04d\n", p.surname, p.day, p.month, p.year);
            found = 1;
        }
    }

    if (found == 0) {
        printf("Людей с таким знаком не найдено.\n");
    }
    fclose(f);
}

// --- ФУНКЦИЯ 4: СОРТИРОВКА ФАЙЛА ПО ДАТЕ РОЖДЕНИЯ ---
void sort_file() {
    FILE* f = fopen(filename, "rb");
    if (f == NULL) {
        printf("Файл пуст или еще не создан.\n");
        return;
    }

    // Узнаем количество записей в файле
    fseek(f, 0, SEEK_END); // Прыгаем в конец файла
    long file_size = ftell(f); // Узнаем размер в байтах
    int count = file_size / sizeof(struct Person); // Делим на размер одной структуры
    rewind(f); // Возвращаемся в начало файла

    if (count < 2) {
        fclose(f);
        printf("Недостаточно данных для сортировки (нужно минимум 2 записи).\n");
        return;
    }

    // Выделяем память под массив всех записей
    struct Person* arr = (struct Person*)malloc(count * sizeof(struct Person));
    if (arr == NULL) {
        fclose(f);
        return;
    }
    fread(arr, sizeof(struct Person), count, f); // Читаем весь файл в массив разом
    fclose(f);

    // Обычная сортировка пузырьком (сравниваем даты через наш лайфхак YYYYMMDD)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            int date1 = arr[j].year * 10000 + arr[j].month * 100 + arr[j].day;
            int date2 = arr[j+1].year * 10000 + arr[j+1].month * 100 + arr[j+1].day;

            if (date1 > date2) {
                // Меняем местами, если дата левого человека больше
                struct Person temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }

    // Открываем файл для ПЕРЕЗАПИСИ ("wb" - write binary)
    f = fopen(filename, "wb");
    if (f != NULL) {
        fwrite(arr, sizeof(struct Person), count, f); // Записываем отсортированный массив
        fclose(f);
    }
    free(arr); // Освобождаем память

    printf("\nФайл успешно отсортирован по дате рождения!\n");
}

int main(void) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int menu;
    do {
        printf("\n================ БАЗА ДАННЫХ ================\n");
        printf("1. Добавить человека\n");
        printf("2. Показать всех\n");
        printf("3. Поиск по знаку Зодиака\n");
        printf("4. Отсортировать базу по дате рождения\n");
        printf("0. Выход\n");
        printf("> ");
        if (scanf("%d", &menu) != 1) {
            if (feof(stdin)) break;
            menu = -1;
        }
        rewind(stdin);

        switch (menu) {
            case 1: add_person(); break;
            case 2: show_all(); break;
            case 3: search_zodiac(); break;
            case 4: sort_file(); break;
            case 0: printf("Завершение программы.\n"); break;
            default: printf("Неверный пункт меню!\n");
        }
    } while (menu != 0);

    return 0;
}