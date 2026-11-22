#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>

#define TYPE_LEN 30
#define LIMIT_HOURS 3.0
#define MINUTES_PER_HOUR 60
#define HOURS_PER_DAY 24
#define FILENAME "rentals.txt"

struct Time {
	int hours;
	int minutes;
};

struct BikeRental {
	int id;
	char type[TYPE_LEN];
	double rate;
	struct Time start;
	struct Time end;
};

struct BikeRental input_rental(int number);
double calculate_duration(struct Time start, struct Time end);
void print_rental(struct BikeRental r, int number);
int print_long_rental(struct BikeRental r, int number, double limit);
int writefile(const char *fname, const char *mode, struct BikeRental r);

int main(void) {
	struct BikeRental r1, r2, r3;
	int found;

	setlocale(LC_ALL, "RUS");

	printf("Программа учета проката велосипедов с записью в файл\n\n");

	r1 = input_rental(1);
	r2 = input_rental(2);
	r3 = input_rental(3);

	printf("Все записи проката\n");
	print_rental(r1, 1);
	print_rental(r2, 2);
	print_rental(r3, 3);

	printf("\nЗаписи с продолжительностью аренды более %.0lf часов\n", LIMIT_HOURS);
	found = 0;
	found += print_long_rental(r1, 1, LIMIT_HOURS);
	found += print_long_rental(r2, 2, LIMIT_HOURS);
	found += print_long_rental(r3, 3, LIMIT_HOURS);
	if (found == 0) {
		printf("Записей с продолжительностью более %.0lf часов не найдено.\n", LIMIT_HOURS);
	}

	if (writefile(FILENAME, "w", r1) &&
		writefile(FILENAME, "a", r2) &&
		writefile(FILENAME, "a", r3)) {
		printf("\nДанные успешно записаны в файл %s\n", FILENAME);
	}
	else {
		printf("\nНе удалось записать данные в файл %s\n", FILENAME);
		return 1;
	}

	return 0;
}

struct BikeRental input_rental(int number) {
	struct BikeRental r;

	printf("Ввод данных для записи %d:\n", number);
	printf("Введите номер велосипеда: ");
	scanf("%d", &r.id);
	printf("Введите тип велосипеда (без пробелов): ");
	scanf("%29s", r.type);
	printf("Введите стоимость аренды за час (руб): ");
	scanf("%lf", &r.rate);
	printf("Введите время начала аренды (часы и минуты): ");
	scanf("%d %d", &r.start.hours, &r.start.minutes);
	printf("Введите время окончания аренды (часы и минуты): ");
	scanf("%d %d", &r.end.hours, &r.end.minutes);
	printf("\n");

	return r;
}

double calculate_duration(struct Time start, struct Time end) {
	int start_min;
	int end_min;
	int diff;

	start_min = start.hours * MINUTES_PER_HOUR + start.minutes;
	end_min = end.hours * MINUTES_PER_HOUR + end.minutes;
	diff = end_min - start_min;

	if (diff < 0) {
		diff += HOURS_PER_DAY * MINUTES_PER_HOUR;
	}

	return (double)diff / MINUTES_PER_HOUR;
}

void print_rental(struct BikeRental r, int number) {
	double duration;
	double cost;

	duration = calculate_duration(r.start, r.end);
	cost = duration * r.rate;

	printf("\nЗапись %d:\n", number);
	printf("  Номер велосипеда: %d\n", r.id);
	printf("  Тип: %s\n", r.type);
	printf("  Тариф: %.2lf руб/ч\n", r.rate);
	printf("  Время аренды: %02d:%02d - %02d:%02d\n", r.start.hours, r.start.minutes, r.end.hours, r.end.minutes);
	printf("  Продолжительность: %.2lf ч\n", duration);
	printf("  Стоимость: %.2lf руб\n", cost);
}

int print_long_rental(struct BikeRental r, int number, double limit) {
	if (calculate_duration(r.start, r.end) > limit) {
		print_rental(r, number);
		return 1;
	}
	return 0;
}

int writefile(const char *fname, const char *mode, struct BikeRental r) {
	FILE *out;
	double duration;
	double cost;

	if ((out = fopen(fname, mode)) == NULL) {
		printf("Ошибка открытия файла для записи\n");
		return 0;
	}

	duration = calculate_duration(r.start, r.end);
	cost = duration * r.rate;

	if (fprintf(out, "Номер:%d ; Тип:%20s ; Тариф:%.2lf ; Начало:%02d:%02d ; Окончание:%02d:%02d ; Продолжительность:%.2lf ; Стоимость:%.2lf\n",
		r.id, r.type, r.rate,
		r.start.hours, r.start.minutes,
		r.end.hours, r.end.minutes,
		duration, cost) < 0) {
		printf("Ошибка записи в файл\n");
		fclose(out);
		return 0;
	}

	if (fclose(out) == EOF) {
		printf("Ошибка закрытия файла\n");
		return 0;
	}

	return 1;
}
