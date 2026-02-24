/**
 * @file math/roots.c
 * @brief Реализация методов поиска корней уравнений
 */

#include "math/roots.h"
#include "ui/utils.h"
#include <math.h>
#include <stdio.h>

/*============================================================================*/
/*                            МЕТОД ДЕЛЕНИЯ ОТРЕЗКА                          */
/*============================================================================*/

float root_find_div(float xl, float xr, float eps, function f, function g, int* iterations) {
    float xm;
    int local_iter = 0;
    
    float fl = f(xl) - g(xl);
    float fr = f(xr) - g(xr);
    
    if (fl * fr > 0) {
        if (iterations != NULL) *iterations = local_iter;
        return NAN;
    }
    
    if (fabsf(fl) < eps) {
        if (iterations != NULL) *iterations = local_iter;
        return xl;
    }
    if (fabsf(fr) < eps) {
        if (iterations != NULL) *iterations = local_iter;
        return xr;
    }
    
    int max_iter = MAX_ROOT_ITERATIONS;
    
    while (fabsf(xr - xl) > eps && local_iter < max_iter) {
        local_iter++;
        xm = (xl + xr) / 2.0f;
        float fm = f(xm) - g(xm);
        
        if (fabsf(fm) < eps) {
            if (iterations != NULL) *iterations = local_iter;
            return xm;
        }
        
        if (fl * fm <= 0) {
            xr = xm;
            fr = fm;
        } else {
            xl = xm;
            fl = fm;
        }
    }
    
    if (iterations != NULL) *iterations = local_iter;
    return (xl + xr) / 2.0f;
}

/*============================================================================*/
/*                            МЕТОД ХОРД                                      */
/*============================================================================*/

float root_find_chord(float xl, float xr, float eps, function f, function g, int* iterations) {
    float x_curr;
    float fl, fr, f_curr;
    
    int local_iter = 0;
    
    fl = f(xl) - g(xl);
    fr = f(xr) - g(xr);
    
    if (fl * fr > 0) {
        if (iterations != NULL) *iterations = local_iter;
        return NAN;
    }
    
    int max_iter = MAX_ROOT_ITERATIONS;
    
    do {
        local_iter++;
        
        x_curr = xl - fl * (xr - xl) / (fr - fl);
        f_curr = f(x_curr) - g(x_curr);
        
        if (fabsf(f_curr) < eps) {
            if (iterations != NULL) *iterations = local_iter;
            return x_curr;
        }
        
        if (fl * f_curr <= 0) {
            xr = x_curr;
            fr = f_curr;
        } else {
            xl = x_curr;
            fl = f_curr;
        }
        
        if (local_iter > max_iter) break;
    } while (fabsf(xr - xl) > eps);
    
    if (iterations != NULL) *iterations = local_iter;
    return (xl + xr) / 2.0f;
}

/*============================================================================*/
/*                            МЕТОД КАСАТЕЛЬНЫХ                              */
/*============================================================================*/

float root_find_tangent(float x0, float eps, function f, function g,
                        derivative df, derivative dg, int* iterations) {
    float x_prev, x_curr = x0;
    float h_x, dh_x;
    float f_prev;
    int max_iter = MAX_TANGENT_ITERATIONS;
    
    int local_iter = 0;
    
    if (isnan(x0) || isinf(x0)) {
        if (iterations != NULL) *iterations = local_iter;
        return NAN;
    }
    
    do {
        local_iter++;
        
        h_x = f(x_curr) - g(x_curr);
        dh_x = df(x_curr) - dg(x_curr);
        
        if (isnan(h_x) || isinf(h_x) || isnan(dh_x) || isinf(dh_x)) {
            break;
        }
        
        if (fabsf(dh_x) < DERIVATIVE_EPS) {
            break;
        }
        
        f_prev = h_x;
        x_prev = x_curr;
        
        x_curr = x_prev - h_x / dh_x;
        
        if (fabsf(x_curr) > RANGE_LIMIT) {
            break;
        }
        
        if (fabsf(h_x) < eps) {
            if (iterations != NULL) *iterations = local_iter;
            return x_curr;
        }
        
        if (fabsf(x_curr - x_prev) < eps * EPS_TANGENT_FACTOR) {
            if (fabsf(h_x) > eps * EPS_COMPARE_FACTOR) {
                break;
            }
        }
        
        float f_new = f(x_curr) - g(x_curr);
        if (fabsf(f_new) > fabsf(f_prev) * DIVERGENCE_LIMIT && local_iter > 2) {
            break;
        }
        
        if (local_iter > max_iter) break;
        
    } while (fabsf(x_curr - x_prev) > eps && fabsf(h_x) > eps);
    
    float final_f = f(x_curr) - g(x_curr);
    if (fabsf(final_f) < eps * EPS_COMPARE_FACTOR) {
        if (iterations != NULL) *iterations = local_iter;
        return x_curr;
    }
    
    if (iterations != NULL) *iterations = local_iter;
    return NAN;
}

/*============================================================================*/
/*                            КОМБИНИРОВАННЫЙ МЕТОД                          */
/*============================================================================*/

float root_find_combined(float xl, float xr, float eps, function f, function g,
                         derivative df, derivative dg, int* iterations) {
    float x_chord, x_tangent;
    float fl, fr;
    float x_prev = xl;
    int max_iter = MAX_TANGENT_ITERATIONS;
    
    int local_iter = 0;
    
    fl = f(xl) - g(xl);
    fr = f(xr) - g(xr);
    
    if (fl * fr > 0) {
        if (iterations != NULL) *iterations = local_iter;
        return NAN;
    }
    
    do {
        local_iter++;
        
        x_prev = (xl + xr) / 2.0f;
        
        x_chord = xl - fl * (xr - xl) / (fr - fl);
        
        float dfr = df(xr) - dg(xr);
        if (fabsf(dfr) < DERIVATIVE_EPS) {
            x_tangent = x_chord;
        } else {
            x_tangent = xr - fr / dfr;
        }
        
        if (x_chord < x_tangent) {
            xl = x_chord;
            xr = x_tangent;
        } else {
            xl = x_tangent;
            xr = x_chord;
        }
        
        fl = f(xl) - g(xl);
        fr = f(xr) - g(xr);
        
        float x_mid = (xl + xr) / 2.0f;
        if (fabsf(x_mid - x_prev) < eps) {
            if (iterations != NULL) *iterations = local_iter;
            return x_mid;
        }
        
        if (fabsf(fl) < eps) {
            if (iterations != NULL) *iterations = local_iter;
            return xl;
        }
        if (fabsf(fr) < eps) {
            if (iterations != NULL) *iterations = local_iter;
            return xr;
        }
        
        if (local_iter > max_iter) break;
        
    } while (fabsf(xr - xl) > eps);
    
    if (iterations != NULL) *iterations = local_iter;
    return (xl + xr) / 2.0f;
}

/*============================================================================*/
/*                            ЕДИНАЯ ФУНКЦИЯ ПОИСКА КОРНЯ                    */
/*============================================================================*/

float root(function f, function g, float a, float b, float eps, int method,
           derivative df, derivative dg, int* iterations) {
    
    int local_iter = 0;
    float result = NAN;
    
    switch(method) {
        case 0:
            result = root_find_div(a, b, eps, f, g, &local_iter);
            break;
            
        case 1:
            result = root_find_chord(a, b, eps, f, g, &local_iter);
            break;
            
        case 2:
            if (!df || !dg) {
                printf("ОШИБКА: Для метода касательных нужны производные!\n");
                if (iterations != NULL) *iterations = 0;
                return NAN;
            }

            float x0;
            float fa = f(a) - g(a);
            float fb = f(b) - g(b);
            
            if (isnan(fa) || isinf(fa)) {
                if (isnan(fb) || isinf(fb)) {
                    if (iterations != NULL) *iterations = 0;
                    return NAN;
                }
                x0 = b;
            } else if (isnan(fb) || isinf(fb)) {
                x0 = a;
            } else {
                x0 = (fabsf(fa) < fabsf(fb)) ? a : b;
            }
            
            result = root_find_tangent(x0, eps, f, g, df, dg, &local_iter);
            
            if (!isnan(result)) {
                if (result < a - F3_SAFE_DISTANCE || result > b + F3_SAFE_DISTANCE) {
                    result = NAN;
                } else {
                    float f_val = f(result) - g(result);
                    if (fabsf(f_val) > eps * EPS_COMPARE_FACTOR) {
                        result = NAN;
                    }
                }
            }
            break;

        case 3:
            if (!df || !dg) {
                printf("ОШИБКА: Для комбинированного метода нужны производные!\n");
                if (iterations != NULL) *iterations = 0;
                return NAN;
            }
            result = root_find_combined(a, b, eps, f, g, df, dg, &local_iter);
            break;
            
        default:
            printf("ОШИБКА: Неизвестный метод %d, используется метод деления\n", method);
            result = root_find_div(a, b, eps, f, g, &local_iter);
            break;
    }
    
    if (iterations != NULL) {
        *iterations = local_iter;
    }
    
    return result;
}