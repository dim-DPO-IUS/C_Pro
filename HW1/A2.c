#include <stdio.h>
#include <stdint.h>

int main(void) {
    uint32_t n;
    unsigned int k;
    
    scanf("%u %u", &n, &k);
    
    // Циклический сдвиг вправо на K битов
    // Если K = 0, то (32 - 0) = 32, что приведет к неопределенному поведению
    // Поэтому обрабатываем K = 0 отдельно
    k %= 32;
    
    uint32_t result;
    if (k == 0) {
        result = n;
    } else {
        result = (n >> k) | (n << (32 - k));
    }
    
    printf("%u\n", result);
    
    return 0;
}
