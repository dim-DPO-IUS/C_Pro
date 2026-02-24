/**
 * @file math/roots.h
 * @brief Методы поиска корней уравнений
 * 
 * Содержит функции для численного решения уравнений вида f(x) = g(x)
 * различными методами: деление отрезка пополам, метод хорд, метод касательных,
 * комбинированный метод.
 */

#ifndef MATH_ROOTS_H
#define MATH_ROOTS_H

#include "constants.h"
#include "math/functions.h"
#include <math.h>
#include <stdio.h>

/*============================================================================*/
/*                            СЧЁТЧИК ИТЕРАЦИЙ                               */
/*============================================================================*/

void root_reset_iterations(void);
void root_set_iterations(int iter);

/*============================================================================*/
/*                            МЕТОД ДЕЛЕНИЯ ОТРЕЗКА                          */
/*============================================================================*/

float root_find_div(float xl, float xr, float eps, function f, function g, int* iterations);

/*============================================================================*/
/*                            МЕТОД ХОРД                                      */
/*============================================================================*/

float root_find_chord(float xl, float xr, float eps, function f, function g, int* iterations);

/*============================================================================*/
/*                            МЕТОД КАСАТЕЛЬНЫХ                              */
/*============================================================================*/

float root_find_tangent(float x0, float eps, function f, function g,
                        derivative df, derivative dg, int* iterations);

/*============================================================================*/
/*                            КОМБИНИРОВАННЫЙ МЕТОД                          */
/*============================================================================*/

float root_find_combined(float xl, float xr, float eps, function f, function g,
                         derivative df, derivative dg, int* iterations);

/*============================================================================*/
/*                            ЕДИНАЯ ФУНКЦИЯ ПОИСКА КОРНЯ                    */
/*============================================================================*/

float root(function f, function g, float a, float b, float eps, int method,
           derivative df, derivative dg, int* iterations);

#endif /* MATH_ROOTS_H */