#include <stdio.h>
#include <string.h>

// Проверяет, является ли pat подпоследовательностью s
int is_subsequence(const char *s, const char *pat) {
    int j = 0;
    for (int i = 0; s[i] && pat[j]; i++) {
        if (s[i] == pat[j]) j++;
    }
    return pat[j] == '\0';
}

int main() {
    char s[1001];
    scanf("%s", s);
    
    int count = 0;
    // Проверяем все трёхзначные числа
    for (int num = 100; num <= 999; num++) {
        char pat[4];
        sprintf(pat, "%d", num);
        if (is_subsequence(s, pat)) {
            count++;
        }
    }
    
    printf("%d\n", count);
    return 0;
}