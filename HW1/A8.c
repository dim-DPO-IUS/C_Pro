#include <stdio.h>
#include <math.h>

int main(void) {
    unsigned long long m;
    scanf("%llu", &m);
    
    // Проверяем, можно ли взвесить такой вес
    // С учетом ограничения чашек (1,000,000 грамм)
    
    int num_weights = 0;
    int iteration = 0;
    unsigned long long temp = m;
    
    while (temp > 0) {
        iteration++;
        
        // Определяем, какая гиря будет использована на этой итерации
        // На i-й итерации рассматривается гиря весом 3^(i-1) грамм
        
        unsigned long long current_weight = 1;
        for (int i = 1; i < iteration; i++) {
            current_weight *= 3;
            // Если гиря превышает 1,000,000 грамм - невозможно взвесить
            if (current_weight > 1000000) {
                printf("-1\n");
                return 0;
            }
        }
        
        int r = temp % 3;
        
        if (r == 1) {
            // Используем гирю current_weight
            if (current_weight > 1000000) {
                printf("-1\n");
                return 0;
            }
            num_weights++;
            temp = (temp - 1) / 3;
        } 
        else if (r == 2) {
            // Используем гирю current_weight (на противоположную чашку)
            if (current_weight > 1000000) {
                printf("-1\n");
                return 0;
            }
            num_weights++;
            temp = (temp + 1) / 3;
        } 
        else {
            temp = temp / 3;
        }
        
        // Защита от бесконечного цикла
        if (iteration > 100) {
            printf("-1\n");
            return 0;
        }
    }
    
    printf("%d\n", num_weights);
    return 0;
}
