#include <stdio.h>

int main(void) {
    unsigned int n, k;
    scanf("%u %u", &n, &k);
    
    unsigned int max = 0;
    
    // Перебираем все позиции
    for (int i = 0; i <= 32 - k; i++) {
        // Создаем маску для K битов на позиции i
        unsigned int mask = ((1u << k) - 1) << i;
        // Извлекаем и сдвигаем обратно
        unsigned int value = (n & mask) >> i;
        
        if (value > max) max = value;
    }
    
    printf("%u\n", max);
    return 0;
}
