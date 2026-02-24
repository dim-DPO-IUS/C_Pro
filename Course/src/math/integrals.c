/**
 * @file math/integrals.c
 * @brief Реализация методов численного интегрирования
 */

#include "math/integrals.h"
#include <math.h>
#include <stdio.h>

/*============================================================================*/
/*                            МЕТОД ПРЯМОУГОЛЬНИКОВ                          */
/*============================================================================*/

float integral_rectangle(float xl, float xr, float eps, function f, int* iterations) {
    int n = 2;
    float h, sum, result = 0, prev_result = 0;
    int max_iter = MAX_INTEGRAL_ITERATIONS;
    
    int local_iter = 0;
    
    do {
        local_iter++;
        h = (xr - xl) / n;
        sum = 0;
        
        for (int i = 0; i < n; i++) {
            sum += f(xl + (i + 0.5f) * h);
        }
        
        result = h * sum;
        
        if (local_iter > 1) {
            if (fabsf(result - prev_result) < eps) break;
        }
        
        prev_result = result;
        n *= 2;
    } while (local_iter < max_iter);
    
    if (iterations != NULL) *iterations = local_iter;
    return result;
}

/*============================================================================*/
/*                            МЕТОД ТРАПЕЦИЙ                                 */
/*============================================================================*/

float integral_trapezoid(float xl, float xr, float eps, function f, int* iterations) {
    int n = 2;
    float h, sum, result = 0, prev_result = 0;
    int max_iter = MAX_INTEGRAL_ITERATIONS;
    
    int local_iter = 0;
    
    do {
        local_iter++;
        h = (xr - xl) / n;
        sum = 0;
        
        for (int i = 1; i < n; i++) {
            sum += f(xl + i * h);
        }
        
        result = h * ((f(xl) + f(xr)) / 2.0f + sum);
        
        if (local_iter > 1) {
            if (fabsf(result - prev_result) < eps) break;
        }
        
        prev_result = result;
        n *= 2;
    } while (local_iter < max_iter);
    
    if (iterations != NULL) *iterations = local_iter;
    return result;
}

/*============================================================================*/
/*                            МЕТОД СИМПСОНА                                 */
/*============================================================================*/

float integral_simpson(float xl, float xr, float eps, function f, int* iterations) {
    int n = 2;
    float h, sum_odd, sum_even, result = 0, prev_result = 0;
    int max_iter = MAX_INTEGRAL_ITERATIONS;
    
    int local_iter = 0;
    
    do {
        local_iter++;
        h = (xr - xl) / n;
        sum_odd = 0;
        sum_even = 0;
        
        for (int i = 1; i < n; i += 2) {
            sum_odd += f(xl + i * h);
        }
        
        for (int i = 2; i < n; i += 2) {
            sum_even += f(xl + i * h);
        }
        
        result = h / 3 * (f(xl) + f(xr) + 4 * sum_odd + 2 * sum_even);
        
        if (local_iter > 1) {
            if (fabsf(result - prev_result) < eps) break;
        }
        
        prev_result = result;
        n *= 2;
    } while (local_iter < max_iter);
    
    if (iterations != NULL) *iterations = local_iter;
    return result;
}

/*============================================================================*/
/*                            ЕДИНАЯ ФУНКЦИЯ ИНТЕГРИРОВАНИЯ                  */
/*============================================================================*/

float integral(function f, float a, float b, float eps, int method, int* iterations) {
    
    float xl = fminf(a, b);
    float xr = fmaxf(a, b);
    
    int local_iter = 0;
    float result = 0;
    
    switch(method) {
        case 0:
            result = integral_rectangle(xl, xr, eps, f, &local_iter);
            break;
            
        case 1:
            result = integral_trapezoid(xl, xr, eps, f, &local_iter);
            break;
            
        case 2:
            result = integral_simpson(xl, xr, eps, f, &local_iter);
            break;
            
        default:
            printf("ОШИБКА: Неизвестный метод %d, используется метод трапеций\n", method);
            result = integral_trapezoid(xl, xr, eps, f, &local_iter);
            break;
    }
    
    if (iterations != NULL) *iterations = local_iter;
    return result;
}