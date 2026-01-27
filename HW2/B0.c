#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
 Описана структура данных  

typedef struct list {  
uint64_t address;  
size_t size;  
char comment[64];  
struct list *next;  
} list;  

Требуется реализовать только одну функцию, которая в данном списке находит 
адрес блока памяти занимающий больше всего места.  
Адрес хранится в поле address, поле size - соответствующий размер данного 
блока. Если список пустой, то функция должна возвращать 0. Если есть 
несколько таких блоков, то вернуть адрес любого из них.  
Прототип функции:

uint64_t findMaxBlock(list *head)

*/

// Структура для хранения информации о блоке памяти
typedef struct list {
    uint64_t address;    // Адрес блока памяти
    size_t size;         // Размер блока памяти в байтах
    char comment[64];    // Комментарий к блоку (не используется в этой задаче)
    struct list *next;   // Указатель на следующий элемент списка
} list;

// Функция для поиска адреса блока с максимальным размером
uint64_t findMaxBlock(list *head) {
    // Если список пустой, возвращаем 0
    if (head == NULL) {
        return 0;
    }
    
    uint64_t maxAddress = head->address;  // Адрес блока с максимальным размером
    size_t maxSize = head->size;          // Текущий максимальный размер
    
    // Начинаем со второго элемента списка
    list *current = head->next;
    
    // Проходим по всем элементам списка
    while (current != NULL) {
        // Если нашли блок с размером больше текущего максимума
        if (current->size > maxSize) {
            maxSize = current->size;       // Обновляем максимальный размер
            maxAddress = current->address; // Обновляем адрес
        }
        // Переходим к следующему элементу
        current = current->next;
    }
    
    return maxAddress;  
}

int main() {
    int n;                // Количество блоков памяти
    list *head = NULL;    // Указатель на начало списка
    list *tail = NULL;    // Указатель на конец списка
    
    // Чтение количества блоков
    scanf("%d", &n);
    
    // Чтение информации о каждом блоке
    for (int i = 0; i < n; i++) {
        uint64_t addr;    // Адрес блока
        size_t size;      // Размер блока
        
        // Чтение адреса и размера
        scanf("%lu %zu", &addr, &size);
        
        // Выделение памяти для нового узла
        list *newNode = (list*)malloc(sizeof(list));
        
        // Инициализация полей узла
        newNode->address = addr;   // Сохраняем адрес
        newNode->size = size;      // Сохраняем размер
        newNode->next = NULL;      // Новый узел пока ни на что не указывает
        
        // Добавление узла в конец списка
        if (head == NULL) {
            // Если список пуст, новый узел становится головой и хвостом
            head = newNode;
            tail = newNode;
        } else {
            // Если список не пуст, добавляем узел после текущего хвоста
            tail->next = newNode;
            tail = newNode;
        }
    }
    
    // Вызов функции и вывод результата
    uint64_t result = findMaxBlock(head);
    printf("%lu\n", result);
    
    // Освобождение памяти, выделенной под узлы списка
    while (head != NULL) {
        list *temp = head;      // Сохраняем указатель на текущий узел
        head = head->next;      // Переходим к следующему узлу
        free(temp);             // Освобождаем память текущего узла
    }
    
    return 0;
}
