#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    int size;
    int capacity;
} IntList;

void initList(IntList *list) {
    list->capacity = 4;
    list->size = 0;
    list->data = (int *)malloc(sizeof(int) * list->capacity);
}

void resizeList(IntList *list) {
    list->capacity *= 2;
    list->data = (int *)realloc(list->data, sizeof(int) * list->capacity);
}

void insert(IntList *list, int value) {
    if (list->size == list->capacity)
        resizeList(list);
    list->data[list->size++] = value;
}

int removeItem(IntList *list, int value) {
    for (int i = 0; i < list->size; i++) {
        if (list->data[i] == value) {
            for (int j = i; j < list->size - 1; j++)
                list->data[j] = list->data[j + 1];
            list->size--;
            return 1;
        }
    }
    return 0;
}

int get(IntList *list, int index) {
    if (index >= 0 && index < list->size)
        return list->data[index];
    printf("Index out of bounds\n");
    return -1;
}

int getSize(IntList *list) {
    return list->size;
}

int isEmpty(IntList *list) {
    return list->size == 0;
}

void clear(IntList *list) {
    list->size = 0;
}

void printList(IntList *list) {
    for (int i = 0; i < list->size; i++)
        printf("%d ", list->data[i]);
    printf("\n");
}

int compareAsc(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int compareDesc(const void *a, const void *b) {
    return (*(int *)b - *(int *)a);
}

void sortAscending(IntList *list) {
    qsort(list->data, list->size, sizeof(int), compareAsc);
}

void sortDescending(IntList *list) {
    qsort(list->data, list->size, sizeof(int), compareDesc);
}

int findMax(IntList *list) {
    if (list->size == 0) return -1;
    int max = list->data[0];
    for (int i = 1; i < list->size; i++)
        if (list->data[i] > max)
            max = list->data[i];
    return max;
}

void freeList(IntList *list) {
    free(list->data);
    list->data = NULL;
}

int main() {
    IntList list;
    initList(&list);

    insert(&list, 10);
    insert(&list, 20);
    insert(&list, 5);
    insert(&list, 30);

    printList(&list); // 10 20 5 30

    removeItem(&list, 20);
    printList(&list); // 10 5 30

    printf("Index 1: %d\n", get(&list, 1)); // 5
    printf("Max: %d\n", findMax(&list));   // 30

    sortAscending(&list);
    printList(&list); // 5 10 30

    sortDescending(&list);
    printList(&list); // 30 10 5

    clear(&list);
    printf("Empty? %s\n", isEmpty(&list) ? "Yes" : "No");

    freeList(&list);
    return 0;
}
