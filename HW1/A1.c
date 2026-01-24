#include <stdio.h>

int main(void) {
    int n;
    scanf("%d ", &n);  // Пробел после %d потребляет пробел/перевод строки
    
    char c;
    while ((c = getchar()) != '.') {
        if (c == ' ') {
            putchar(' ');
        } else if (c >= 'A' && c <= 'Z') {
            putchar('A' + (c - 'A' + n) % 26);
        } else if (c >= 'a' && c <= 'z') {
            putchar('a' + (c - 'a' + n) % 26);
        }
    }
    putchar('.');
    putchar('\n');
    
    return 0;
}
