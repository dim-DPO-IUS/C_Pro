#include <stdio.h>

int main(void) {
    int n;
    scanf("%d", &n);
    
    long long xor_result = 0;  // Используем long long для больших чисел
    
    // Алгоритм:
    // 1. Все числа, встречающиеся 2 раза, при XOR дадут 0 (A ^ A = 0)
    // 2. Число, встречающееся 1 раз, останется (B ^ 0 = B)
    
    for (int i = 0; i < n; i++) {
        long long num;
        scanf("%lld", &num);
        xor_result ^= num;  // Побитовое исключающее ИЛИ
    }
    
    printf("%lld\n", xor_result);
    
    return 0;
}
