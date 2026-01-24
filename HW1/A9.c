#include <stdio.h>
#include <stdint.h>

struct pack_array {
    uint32_t array;      // поле для хранения упакованного массива из 0 и 1
    uint32_t count0 : 8; // счетчик нулей в array  
    uint32_t count1 : 8; // счетчик единиц в array
};

void array2struct(int arr[], struct pack_array *pack) {
    // Инициализация
    pack->array = 0;
    pack->count0 = 0;
    pack->count1 = 0;
    
    // Проходим по всем 32 элементам массива
    for (int i = 0; i < 32; i++) {
        if (arr[i] == 1) {
            // Устанавливаем соответствующий бит
            // arr[0] становится старшим битом (бит 31)
            // arr[31] становится младшим битом (бит 0)
            pack->array |= (1u << (31 - i));
            pack->count1++;
        } else {
            pack->count0++;
        }
    }
}

// Функция для тестирования
int main(void) {
    int arr[32];
    
    // Чтение входных данных
    for (int i = 0; i < 32; i++) {
        scanf("%d", &arr[i]);
    }
    
    struct pack_array pack;
    array2struct(arr, &pack);
    
    // Вывод результата
    printf("%u %u %u\n", pack.array, pack.count0, pack.count1);
    
    return 0;
}
