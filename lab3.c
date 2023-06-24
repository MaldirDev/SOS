#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATE_LENGTH 11

typedef struct {
    char date[MAX_DATE_LENGTH];
    int count;
} LogEntry;

void updateCount(LogEntry* entries, int size, const char* date) {
    int i;
    for (i = 0; i < size; i++) {
        if (strcmp(entries[i].date, date) == 0) {
            entries[i].count++;
            return;
        }
    }

    // Дата не знайдена, додати новий запис
    strcpy(entries[size].date, date);
    entries[size].count = 1;
}

int compareEntries(const void* a, const void* b) {
    int countA = ((LogEntry*)a)->count;
    int countB = ((LogEntry*)b)->count;

    return countB - countA;
}

void printTopDates(LogEntry* entries, int size) {
    int i;
    int totalCount = 0;

    // Загальна кількість звернень
    for (i = 0; i < size; i++) {
        totalCount += entries[i].count;
    }

    printf("Top 10 Dates:\n");
    for (i = 0; i < size && i < 10; i++) {
        double percentage = (entries[i].count / (double)totalCount) * 100;
        printf("%d. %s - %d - %.2lf%%\n", i + 1, entries[i].date, entries[i].count, percentage);
    }
}

char* extractDate(const char* line) {
    const char* dateStart = strstr(line, "[") + 1; // Знаходимо позицію початку дати
    char* date = (char*)malloc(MAX_DATE_LENGTH * sizeof(char));
    strncpy(date, dateStart, MAX_DATE_LENGTH - 1);
    date[MAX_DATE_LENGTH - 1] = '\0'; // Завершуємо рядок нуль-термінатором

    return date;
}

int main() {
    FILE* file = fopen("01-tiny-access.log", "r");
    if (file == NULL) {
        perror("Unable to open file");
        return 1;
    }

    LogEntry entries[100]; // Припустимо, що максимальна кількість різних дат - 100
    int entryCount = 0;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Отримання дати з рядка
        char* date = extractDate(line);

        updateCount(entries, entryCount, date);

        free(date); // Звільняємо пам'ять, виділену для дати
    }

    fclose(file);

    // Сортування в порядку спадання
    qsort(entries, entryCount, sizeof(LogEntry), compareEntries);

    // Виведення топ-10 дат
    printTopDates(entries, entryCount);

    return 0;
}
