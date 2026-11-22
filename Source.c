#include <stdio.h>
#include <string.h>
#include <locale.h>
#ifdef _WIN32
#include <windows.h>
#endif

#define N 3
#define FILENAME "cars.txt"

struct Date {
    int day;
    int month;
    int year;
};

struct Car {
    char brand[50];
    char manufacturer[50];
    char type[50];
    int release_year;
    struct Date reg_date;
};

void input_car(struct Car *car, int number) {
    printf("Автомобиль %d\n", number);
    printf("Марка автомобиля: ");
    scanf("%49s", car->brand);
    printf("Производитель: ");
    scanf("%49s", car->manufacturer);
    printf("Тип: ");
    scanf("%49s", car->type);
    printf("Год выпуска: ");
    scanf("%d", &car->release_year);
    printf("Дата регистрации (день месяц год): ");
    scanf("%d %d %d", &car->reg_date.day, &car->reg_date.month, &car->reg_date.year);
}

void print_car(const struct Car *car) {
    printf("Марка: %s\n", car->brand);
    printf("Производитель: %s\n", car->manufacturer);
    printf("Тип: %s\n", car->type);
    printf("Год выпуска: %d\n", car->release_year);
    printf("Дата регистрации: %02d.%02d.%04d\n",
           car->reg_date.day, car->reg_date.month, car->reg_date.year);
}

void process_cars(const struct Car cars[], int n) {
    int found = 0;

    for (int i = 0; i < n; i++) {
        if (strcmp(cars[i].brand, "Ford") == 0 && cars[i].reg_date.year < 2000) {
            found++;
        }
    }

    printf("\nНайдено автомобилей марки Ford, зарегистрированных до 2000 года: %d\n", found);

    for (int i = 0; i < n; i++) {
        if (strcmp(cars[i].brand, "Ford") == 0 && cars[i].reg_date.year < 2000) {
            printf("\n");
            print_car(&cars[i]);
        }
    }
}

int writefile(char *fname, const struct Car cars[], int size) {
    FILE *out;

    if ((out = fopen(fname, "wt")) == NULL) {
        printf("Ошибка открытия файла для записи\n");
        return 0;
    }

    for (int i = 0; i < size; i++) {
        fprintf(out, "Марка:%s ; Производитель:%s ; Тип:%s ; Год:%d ; Дата:%02d.%02d.%04d\n",
                cars[i].brand, cars[i].manufacturer, cars[i].type, cars[i].release_year,
                cars[i].reg_date.day, cars[i].reg_date.month, cars[i].reg_date.year);
    }

    fclose(out);
    return 1;
}

int main(void) {
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    setlocale(LC_ALL, ".UTF8");
#else
    setlocale(LC_ALL, ".UTF8");
#endif

    struct Car cars[N];

    for (int i = 0; i < N; i++) {
        input_car(&cars[i], i + 1);
    }

    process_cars(cars, N);

    if (writefile(FILENAME, cars, N)) {
        printf("\nДанные успешно записаны в файл %s\n", FILENAME);
    }
    else {
        printf("\nОшибка записи данных в файл\n");
    }

    return 0;
}
