/**
 * @file geometry/pointfinder.c
 * @brief Реализация поиска точек пересечения функций
 */

#include "geometry/pointfinder.h"
#include <math.h>
#include <stdio.h>

/*============================================================================*/
/*                            ПОИСК ИНТЕРВАЛОВ С КОРНЯМИ                     */
/*============================================================================*/

int find_intervals(float xmin, float xmax, int steps, function h, 
                   Interval intervals[], int max_intervals) {
    float dx = (xmax - xmin) / steps;
    float x_prev = xmin;
    float val_prev = h(x_prev);
    int count = 0;
    
    if (isnan(val_prev)) {
        x_prev += dx * EPS_TANGENT_FACTOR;
        val_prev = h(x_prev);
        if (isnan(val_prev)) return 0;
    }
    
    for (int i = 1; i <= steps && count < max_intervals; i++) {
        float x_curr = xmin + i * dx;
        float val_curr = h(x_curr);
        
        if (isnan(val_curr)) {
            x_curr += dx * EPS_TANGENT_FACTOR;
            val_curr = h(x_curr);
            if (isnan(val_curr)) continue;
        }
        
        if (val_prev * val_curr < 0) {
            intervals[count].left = x_prev;
            intervals[count].right = x_curr;
            count++;
        }
        
        x_prev = x_curr;
        val_prev = val_curr;
    }
    
    return count;
}

/*============================================================================*/
/*                            СОРТИРОВКА ТОЧЕК                               */
/*============================================================================*/

void sort_points(Point points[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (points[j].x < points[i].x) {
                Point temp = points[i];
                points[i] = points[j];
                points[j] = temp;
            }
        }
    }
}

/*============================================================================*/
/*                            ПОИСК ТОЧЕК ПО ТИПУ                             */
/*============================================================================*/

int find_point_by_type(Point points[], int count, int pair_type, int side) {
    for (int i = 0; i < count; i++) {
        if (points[i].pair_type != pair_type) continue;
        
        if (side != 0) {
            if (side == -1 && points[i].x >= 0) continue;
            if (side == 1 && points[i].x <= 0) continue;
        }
        
        return i;
    }
    return -1;
}

int find_all_points_by_type(Point points[], int count, int pair_type, int indices[]) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (points[i].pair_type == pair_type) {
            indices[found++] = i;
        }
    }
    return found;
}

/*============================================================================*/
/*                            ПОИСК ВСЕХ ТОЧЕК ПЕРЕСЕЧЕНИЯ                   */
/*============================================================================*/

int find_all_intersections(float xmin, float xmax, int steps, float eps_root,
                           Point points[], int max_points, int root_method) {
    Interval intervals12[MAX_INTERVALS];
    Interval intervals13[MAX_INTERVALS];
    Interval intervals23[MAX_INTERVALS];
    
    int count12, count13, count23;
    int point_count = 0;
    
    count12 = find_intervals(xmin, xmax, steps, diff_f1_f2, 
                             intervals12, MAX_INTERVALS);
    count13 = find_intervals(xmin, xmax, steps, diff_f1_f3, 
                             intervals13, MAX_INTERVALS);
    count23 = find_intervals(xmin, xmax, steps, diff_f2_f3, 
                             intervals23, MAX_INTERVALS);
    
    // f1-f2
    for (int i = 0; i < count12 && point_count < max_points; i++) {
        int iter = 0;
        float x = root(f1, f2, 
                    intervals12[i].left, intervals12[i].right, 
                    eps_root, root_method, df1, df2, &iter);
        
        if (!isnan(x) && utils_is_valid(x)) {
            float y = f1(x);
            float y2 = f2(x);
            
            if (fabsf(y - y2) < eps_root * EPS_COMPARE_FACTOR) {
                points[point_count].x = x;
                points[point_count].y = y;
                points[point_count].pair_type = 1;
                points[point_count].iterations = iter;
                point_count++;
                
                if (DEBUG_MODE) {
                    printf("  Найден корень f1-f2: x=%f, y=%f, итераций=%d\n", x, y, iter);
                }
            }
        }
    }
    
    // f1-f3
    for (int i = 0; i < count13 && point_count < max_points; i++) {
        if (intervals13[i].left < 0 && intervals13[i].right > 0) {
            continue;
        }
        
        int iter = 0;
        float x = root(f1, f3,
                    intervals13[i].left, intervals13[i].right,
                    eps_root, root_method, df1, df3, &iter);
        
        if (!isnan(x) && fabsf(x) > F3_SAFE_DISTANCE && utils_is_valid(x)) {
            float y1 = f1(x);
            float y3 = f3(x);
            
            if (fabsf(y1 - y3) < eps_root * EPS_COMPARE_FACTOR) {
                points[point_count].x = x;
                points[point_count].y = y1;
                points[point_count].pair_type = 2;
                points[point_count].iterations = iter;
                point_count++;
                
                if (DEBUG_MODE) {
                    printf("  Найден корень f1-f3: x=%f, y=%f, итераций=%d\n", x, y1, iter);
                }
            }
        }
    }
    
    // f2-f3
    for (int i = 0; i < count23 && point_count < max_points; i++) {
        if (intervals23[i].left < 0 && intervals23[i].right > 0) {
            continue;
        }
        
        int iter = 0;
        float x = root(f2, f3,
                    intervals23[i].left, intervals23[i].right,
                    eps_root, root_method, df2, df3, &iter);
        
        if (!isnan(x) && fabsf(x) > F3_SAFE_DISTANCE && utils_is_valid(x)) {
            float y2 = f2(x);
            float y3 = f3(x);
            
            if (fabsf(y2 - y3) < eps_root * EPS_COMPARE_FACTOR) {
                points[point_count].x = x;
                points[point_count].y = y2;
                points[point_count].pair_type = 3;
                points[point_count].iterations = iter;
                point_count++;
                
                if (DEBUG_MODE) {
                    printf("  Найден корень f2-f3: x=%f, y=%f, итераций=%d\n", x, y2, iter);
                }
            }
        }
    }
        
    // Специальная обработка для интервалов, пересекающих x=0
    // f1-f3
    for (int i = 0; i < count13 && point_count < max_points; i++) {
        if (intervals13[i].left < 0 && intervals13[i].right > 0) {
            int iter_left = 0, iter_right = 0;
            
            float x_left = root(f1, f3,
                            intervals13[i].left, -F3_SAFE_DISTANCE,
                            eps_root, root_method, df1, df3, &iter_left);
            
            if (!isnan(x_left) && fabsf(x_left) > F3_SAFE_DISTANCE) {
                points[point_count].x = x_left;
                points[point_count].y = f1(x_left);
                points[point_count].pair_type = 2;
                points[point_count].iterations = iter_left;
                point_count++;
            }
            
            float x_right = root(f1, f3,
                                F3_SAFE_DISTANCE, intervals13[i].right,
                                eps_root, root_method, df1, df3, &iter_right);
            
            if (!isnan(x_right) && fabsf(x_right) > F3_SAFE_DISTANCE) {
                points[point_count].x = x_right;
                points[point_count].y = f1(x_right);
                points[point_count].pair_type = 2;
                points[point_count].iterations = iter_right;
                point_count++;
            }
        }
    }

    // f2-f3
    for (int i = 0; i < count23 && point_count < max_points; i++) {
        if (intervals23[i].left < 0 && intervals23[i].right > 0) {
            int iter_left = 0, iter_right = 0;
            
            float x_left = root(f2, f3,
                            intervals23[i].left, -F3_SAFE_DISTANCE,
                            eps_root, root_method, df2, df3, &iter_left);
            
            if (!isnan(x_left) && fabsf(x_left) > F3_SAFE_DISTANCE) {
                points[point_count].x = x_left;
                points[point_count].y = f2(x_left);
                points[point_count].pair_type = 3;
                points[point_count].iterations = iter_left;
                point_count++;
            }
            
            float x_right = root(f2, f3,
                                F3_SAFE_DISTANCE, intervals23[i].right,
                                eps_root, root_method, df2, df3, &iter_right);
            
            if (!isnan(x_right) && fabsf(x_right) > F3_SAFE_DISTANCE) {
                points[point_count].x = x_right;
                points[point_count].y = f2(x_right);
                points[point_count].pair_type = 3;
                points[point_count].iterations = iter_right;
                point_count++;
            }
        }
    }

    return point_count;
}