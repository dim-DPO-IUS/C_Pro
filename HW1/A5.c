#include <stdio.h>

int main(void) {
    unsigned int n;
    scanf("%u", &n);
    
    int count = 0;
    
    while (n) {
        // Проверяем младший бит
        if (n & 1) {
            count++;
        }
        // Сдвигаем число вправо
        n >>= 1;
    }
    
    printf("%d\n", count);
    return 0;
}
