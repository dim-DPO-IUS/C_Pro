#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STACK_SIZE 1000

int stack[STACK_SIZE];
int top = -1;

void push(int value) {
    if (top >= STACK_SIZE - 1) {
        fprintf(stderr, "Stack overflow\n");
        exit(1);
    }
    stack[++top] = value;
}

int pop() {
    if (top < 0) {
        fprintf(stderr, "Stack underflow\n");
        exit(1);
    }
    return stack[top--];
}

int is_operator(const char *token) {
    // Оператор может быть строкой из одного символа +, -, *, /
    // Или строкой вида "+." где точка сразу после оператора
    if (token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '/') {
        // Если второй символ существует и это не точка или конец строки
        if (token[1] == '\0' || token[1] == '.') {
            return 1;
        }
    }
    return 0;
}

int main() {
    char input[10000];
    fgets(input, sizeof(input), stdin);
    
    // Используем strtok с пробелами и точкой как разделителями
    char *token = strtok(input, " .\n");
    
    while (token != NULL) {
        // Если токен - оператор (возможно с точкой на конце)
        if (is_operator(token)) {
            char op = token[0];
            
            if (top < 1) {
                fprintf(stderr, "Not enough operands for operator %c\n", op);
                exit(1);
            }
            
            int b = pop();
            int a = pop();
            int result;
            
            switch (op) {
                case '+': result = a + b; break;
                case '-': result = a - b; break;
                case '*': result = a * b; break;
                case '/': 
                    if (b == 0) {
                        fprintf(stderr, "Division by zero\n");
                        exit(1);
                    }
                    result = a / b; 
                    break;
                default:
                    fprintf(stderr, "Unknown operator: %c\n", op);
                    exit(1);
            }
            
            push(result);
        } 
        else {
            // Токен - число
            int num = atoi(token);
            push(num);
        }
        
        token = strtok(NULL, " .\n");
    }
    
    // В стеке должен остаться ровно один элемент - результат
    if (top != 0) {
        fprintf(stderr, "Invalid expression\n");
        exit(1);
    }
    
    int result = pop();
    
    printf("%d\n", result);
    return 0;
}