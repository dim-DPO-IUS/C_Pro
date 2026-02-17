#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
// #include "D1.h"


// Структура для элемента очереди
typedef struct QueueNode {
    tree *node;
    int hd;
    struct QueueNode *next;
} QueueNode;

// Структура для хранения узла с информацией
typedef struct MapNode {
    int hd;
    int value;
    int level;
    struct MapNode *next;
} MapNode;

// Функции для работы с очередью
void enqueue(QueueNode **front, QueueNode **rear, tree *node, int hd) {
    QueueNode *newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->node = node;
    newNode->hd = hd;
    newNode->next = NULL;
    
    if (*rear == NULL) {
        *front = *rear = newNode;
        return;
    }
    (*rear)->next = newNode;
    *rear = newNode;
}

QueueNode* dequeue(QueueNode **front, QueueNode **rear) {
    if (*front == NULL) return NULL;
    
    QueueNode *temp = *front;
    *front = (*front)->next;
    
    if (*front == NULL) *rear = NULL;
    
    return temp;
}

// Функции для работы с картой
void updateMap(MapNode **map, int hd, int value, int level) {
    MapNode *current = *map;
    MapNode *prev = NULL;
    
    // Ищем существующий узел с таким hd
    while (current != NULL) {
        if (current->hd == hd) {
            if (level < current->level) {
                current->value = value;
                current->level = level;
            }
            return;
        }
        prev = current;
        current = current->next;
    }
    
    // Создаем новый узел
    MapNode *newNode = (MapNode*)malloc(sizeof(MapNode));
    newNode->hd = hd;
    newNode->value = value;
    newNode->level = level;
    newNode->next = NULL;
    
    if (*map == NULL) {
        *map = newNode;
    } else {
        prev->next = newNode;
    }
}

// Сортировка карты по hd
void sortMap(MapNode **map) {
    if (*map == NULL || (*map)->next == NULL) return;
    
    MapNode *i, *j;
    for (i = *map; i != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (i->hd > j->hd) {
                // Меняем местами данные
                int tempHd = i->hd;
                int tempVal = i->value;
                int tempLvl = i->level;
                
                i->hd = j->hd;
                i->value = j->value;
                i->level = j->level;
                
                j->hd = tempHd;
                j->value = tempVal;
                j->level = tempLvl;
            }
        }
    }
}

// Функция для печати вида сверху - ТОЛЬКО ЭТО НУЖНО ОТПРАВИТЬ!
void btUpView(tree *root) {
    if (root == NULL) return;
    
    QueueNode *front = NULL, *rear = NULL;
    MapNode *map = NULL;
    
    // Начинаем обход с корня
    enqueue(&front, &rear, root, 0);
    int level = 0;
    
    while (front != NULL) {
        int levelSize = 0;
        QueueNode *temp = front;
        
        // Считаем количество узлов на текущем уровне
        while (temp != NULL) {
            levelSize++;
            temp = temp->next;
        }
        
        // Обрабатываем все узлы текущего уровня
        for (int i = 0; i < levelSize; i++) {
            QueueNode *current = dequeue(&front, &rear);
            
            if (current != NULL) {
                // Обновляем карту
                updateMap(&map, current->hd, current->node->key, level);
                
                // Добавляем детей
                if (current->node->left != NULL) {
                    enqueue(&front, &rear, current->node->left, current->hd - 1);
                }
                if (current->node->right != NULL) {
                    enqueue(&front, &rear, current->node->right, current->hd + 1);
                }
                
                free(current);
            }
        }
        level++;
    }
    
    // Сортируем и выводим результат
    sortMap(&map);
    
    MapNode *current = map;
    int first = 1;
    while (current != NULL) {
        if (!first) {
            printf(" ");
        }
        printf("%d", current->value);
        first = 0;
        
        MapNode *temp = current;
        current = current->next;
        free(temp);
    }
}