#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct list {
    char word[20];
    struct list *next;
};

// Функция добавления элемента в конец списка
struct list* add_to_list(struct list *head, char *word) {
    struct list *new_node = (struct list*)malloc(sizeof(struct list));
    struct list *current = head;
    
    strcpy(new_node->word, word);
    new_node->next = NULL;
    
    if (head == NULL) {
        return new_node;
    }
    
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
    
    return head;
}

// Функция обмена элементов (меняет местами значения слов в двух узлах)
void swap_elements(struct list *a, struct list *b) {
    char temp[20];
    strcpy(temp, a->word);
    strcpy(a->word, b->word);
    strcpy(b->word, temp);
}

// Функция сортировки списка (пузырьковая сортировка)
void sort_list(struct list *head) {
    if (head == NULL) return;
    
    int swapped;
    struct list *ptr1;
    struct list *lptr = NULL;
    
    do {
        swapped = 0;
        ptr1 = head;
        
        while (ptr1->next != lptr) {
            if (strcmp(ptr1->word, ptr1->next->word) > 0) {
                swap_elements(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

// Функция вывода списка
void print_list(struct list *head) {
    struct list *current = head;
    while (current != NULL) {
        printf("%s ", current->word);
        current = current->next;
    }
    printf("\n");
}

// Функция удаления списка
void delete_list(struct list *head) {
    struct list *current = head;
    struct list *next;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

int main() {
    char input[1000];
    struct list *head = NULL;
    char word[20];
    int i, j, pos = 0;
    
    // Ввод строки
    fgets(input, sizeof(input), stdin);
    
    // Разбор строки на слова
    for (i = 0; input[i] != '.'; i++) {
        if (input[i] != ' ') {
            word[pos++] = input[i];
        } else {
            if (pos > 0) {
                word[pos] = '\0';
                head = add_to_list(head, word);
                pos = 0;
            }
        }
    }
    
    // Добавляем последнее слово перед точкой
    if (pos > 0) {
        word[pos] = '\0';
        head = add_to_list(head, word);
    }
    
    // Сортировка списка
    sort_list(head);
    
    // Вывод результата
    print_list(head);
    
    // Освобождение памяти
    delete_list(head);
    
    return 0;
}