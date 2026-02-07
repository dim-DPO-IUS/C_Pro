#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 20010  // s1(10^4) + s2(10^4) + разделитель

// Z-функция (из лекции)
void compute_z(const char *s, int *z) {
    int n = strlen(s);
    for (int i = 0; i < n; i++) z[i] = 0;
    
    int l = 0, r = 0;
    for (int i = 1; i < n; i++) {
        if (i <= r)
            z[i] = (r - i + 1 < z[i - l]) ? r - i + 1 : z[i - l];
        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
            z[i]++;
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
}

// Находит длину наибольшего префикса a, являющегося суффиксом b
int max_prefix_suffix(const char *a, const char *b) {
    int len_a = strlen(a);
    int len_b = strlen(b);
    
    // Строим строку: a + "#" + b
    char *s = malloc(len_a + 1 + len_b + 1);
    strcpy(s, a);
    strcat(s, "#");
    strcat(s, b);
    
    int *z = malloc((len_a + 1 + len_b) * sizeof(int));
    compute_z(s, z);
    
    int max_len = 0;
    // Ищем суффиксы b (после "#"), которые совпадают с префиксом a
    for (int i = len_a + 1; i < len_a + 1 + len_b; i++) {
        // Если суффикс начинается в позиции i и доходит до конца строки
        if (i + z[i] == len_a + 1 + len_b) {
            if (z[i] > max_len) {
                max_len = z[i];
            }
        }
    }
    
    free(z);
    free(s);
    return max_len;
}

int main() {
    char s1[10001], s2[10001];
    
    // Чтение входных данных
    fgets(s1, sizeof(s1), stdin);
    fgets(s2, sizeof(s2), stdin);
    
    // Удаляем символы новой строки
    s1[strcspn(s1, "\n")] = '\0';
    s2[strcspn(s2, "\n")] = '\0';
    
    // 1) Наибольший префикс s1, являющийся суффиксом s2
    int ans1 = max_prefix_suffix(s1, s2);
    
    // 2) Наибольший суффикс s1, являющийся префиксом s2
    //    Это эквивалентно: наибольший префикс s2, являющийся суффиксом s1
    int ans2 = max_prefix_suffix(s2, s1);
    
    printf("%d %d\n", ans1, ans2);
    return 0;
}