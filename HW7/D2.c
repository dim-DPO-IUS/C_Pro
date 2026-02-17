#include <stdio.h>
#include <stdlib.h>

// typedef struct tree {
//     int key;
//     struct tree *left, *right;
//     struct tree *parent;
// } tree;

tree * findBrother(tree *root, int key) {
    if (root == NULL) return NULL;
    
    // Поиск узла с заданным ключом
    tree *current = root;
    
    // Ищем узел (предполагаем, что дерево - бинарное дерево поиска)
    while (current != NULL && current->key != key) {
        if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    
    // Если узел не найден
    if (current == NULL) return NULL;
    
    // Если узел - корень (нет родителя)
    if (current->parent == NULL) return NULL;
    
    // Определяем брата
    if (current->parent->left == current) {
        // Текущий узел - левый ребенок, брат - правый
        return current->parent->right;
    } else {
        // Текущий узел - правый ребенок, брат - левый
        return current->parent->left;
    }
}