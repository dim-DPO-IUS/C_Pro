/**
 * @file geometry/area_calculator.h
 * @brief Вычисление площади фигур, образованных тремя функциями
 * 
 * Содержит функции для вычисления площади двух фигур путём интегрирования
 * разности конкретных функций, образующих границы фигур.
 */

#ifndef GEOMETRY_AREA_CALCULATOR_H
#define GEOMETRY_AREA_CALCULATOR_H

#include "constants.h"
#include "math/functions.h"
#include "math/integrals.h"
#include "geometry/pointfinder.h"
#include "ui/output.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

/*============================================================================*/
/*                            СТРУКТУРЫ ДЛЯ ОПИСАНИЯ ФИГУР                    */
/*============================================================================*/

/**
 * @brief Сегмент границы фигуры между двумя точками
 */
typedef struct {
    int from;           // индекс начальной точки
    int to;             // индекс конечной точки
    FunctionType upper; // функция сверху
    FunctionType lower; // функция снизу
} BoundarySegment;

/**
 * @brief Описание фигуры
 */
typedef struct {
    const char* name;              // название фигуры
    int n_segments;                // количество сегментов
    BoundarySegment segments[MAX_SEGMENTS];  // сегменты, составляющие фигуру
} FigureDescription;

/*============================================================================*/
/*                            ОСНОВНЫЕ ФУНКЦИИ                                */
/*============================================================================*/

float calculate_segment_area(Point points[], BoundarySegment *seg, 
                             float eps, int int_method, int *iter);

float calculate_figure_area(Point points[], const FigureDescription *figure,
                            float eps, int int_method, int *total_iter);

float calculate_figure1_area(Point points[], int point_count, float eps, 
                            int int_method, int *iterations);

float calculate_figure2_area(Point points[], int point_count, float eps, 
                            int int_method, int *iterations);

#endif /* GEOMETRY_AREA_CALCULATOR_H */