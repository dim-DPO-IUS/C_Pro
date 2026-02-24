/**
 * @file geometry/pointfinder.h
 * @brief Поиск точек пересечения функций
 * 
 * Содержит функции для автоматического поиска всех точек пересечения
 * трёх заданных функций: f1, f2, f3.
 */

#ifndef GEOMETRY_POINTFINDER_H
#define GEOMETRY_POINTFINDER_H

#include "constants.h"
#include "math/functions.h"
#include "math/roots.h"
#include "ui/utils.h"
#include <math.h>
#include <stdio.h>

/*============================================================================*/
/*                            ПОИСК ИНТЕРВАЛОВ С КОРНЯМИ                     */
/*============================================================================*/

int find_intervals(float xmin, float xmax, int steps, function h, 
                   Interval intervals[], int max_intervals);

/*============================================================================*/
/*                            СОРТИРОВКА ТОЧЕК                               */
/*============================================================================*/

void sort_points(Point points[], int count);

/*============================================================================*/
/*                            ПОИСК ТОЧЕК ПО ТИПУ                             */
/*============================================================================*/

int find_point_by_type(Point points[], int count, int pair_type, int side);
int find_all_points_by_type(Point points[], int count, int pair_type, int indices[]);

/*============================================================================*/
/*                            ПОИСК ВСЕХ ТОЧЕК ПЕРЕСЕЧЕНИЯ                   */
/*============================================================================*/

int find_all_intersections(float xmin, float xmax, int steps, float eps_root,
                           Point points[], int max_points, int root_method);

#endif /* GEOMETRY_POINTFINDER_H */