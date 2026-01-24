#include <stdio.h>

int main(void) {
    int n;
    // Читаем только количество чисел
    scanf("%d", &n);
    
    int num;
    int max_value;
    int count = 0;
    
    // Читаем все n чисел без проверок ошибок
    for (int i = 0; i < n; i++) {
        scanf("%d", &num);
        
        if (i == 0) {
            max_value = num;
            count = 1;
        } else if (num > max_value) {
            max_value = num;
            count = 1;
        } else if (num == max_value) {
            count++;
        }
    }
    
    printf("%d\n", count);
    return 0;
}
