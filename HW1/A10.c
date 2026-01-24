#include <stdio.h>
#include <stdint.h>

// Структура для безопасного доступа к битам float
typedef union {
    float f;
    uint32_t u;
} float_bits;

// Функция извлечения порядка из float
int extractExp(float f) {
    float_bits converter;
    converter.f = f;
    
    // Извлекаем поле порядка (биты 23-30)
    return (converter.u >> 23) & 0xFF;
}

int main(void) {
    float number;
    
    // Чтение числа из стандартного ввода
    if (scanf("%f", &number) != 1) {
        printf("Ошибка ввода\n");
        return 1;
    }
    
    // Вызов функции и вывод результата
    int exponent = extractExp(number);
    printf("%d\n", exponent);
    
    return 0;
}
