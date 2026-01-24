#include <stdio.h>
#include <stdint.h>

int main(void) {
    uint32_t n;
    scanf("%u", &n);
    
    // Старший байт - это биты с 24 по 31 (включительно)
    // Младшие 3 байта - биты с 0 по 23
    
    // XOR с 1 инвертирует бит, XOR с 0 оставляет без изменений
    uint32_t mask = 0xFF000000;  // 11111111000000000000000000000000
    uint32_t result = n ^ mask;
    
    printf("%u\n", result);
    
    return 0;
}
