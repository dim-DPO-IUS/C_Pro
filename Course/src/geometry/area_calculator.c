/**
 * @file geometry/area_calculator.c
 * @brief Реализация вычисления площади фигур
 */

#include "geometry/area_calculator.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

/*============================================================================*/
/*                            ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ                        */
/*============================================================================*/

/**
 * @brief Получить функцию разности для пары функций
 * @param upper Верхняя функция
 * @param lower Нижняя функция
 * @return Указатель на функцию разности или NULL
 */
static float (*get_diff_function(FunctionType upper, FunctionType lower))(float) {
    if (upper == FUNC_F1 && lower == FUNC_F2) return diff_f1_f2;
    if (upper == FUNC_F1 && lower == FUNC_F3) return diff_f1_f3;
    if (upper == FUNC_F2 && lower == FUNC_F3) return diff_f2_f3;
    return NULL;
}

/**
 * @brief Проверка, пересекает ли интервал точку x=0
 * @param x1 Левая граница
 * @param x2 Правая граница
 * @return true если интервал содержит 0
 */
static bool crosses_zero(float x1, float x2) {
    return (x1 < 0 && x2 > 0) || (x1 > 0 && x2 < 0);
}

static float calculate_area_with_break(float a, float b, float (*diff)(float), 
                                       float eps, int int_method, int *iter) {
    float left = fminf(a, b);
    float right = fmaxf(a, b);
    
    if (!crosses_zero(left, right)) {
        int local_iter = 0;
        float area = integral(diff, left, right, eps, int_method, &local_iter);
        *iter = local_iter;
        return area;
    }
    
    float dist_to_zero_left = fabsf(left);
    float dist_to_zero_right = fabsf(right);
    float min_dist = fminf(dist_to_zero_left, dist_to_zero_right);
    
    float offset = fminf(BREAK_OFFSET_MAX, 
                        fmaxf(BREAK_OFFSET_MIN, 
                             min_dist * BREAK_OFFSET_FACTOR));
    
    float mid1 = -offset;
    float mid2 = offset;
    
    float total = 0.0f;
    int total_iter = 0;
    
    if (left < mid1) {
        int local_iter = 0;
        total += integral(diff, left, mid1, eps, int_method, &local_iter);
        total_iter += local_iter;
    }
    
    if (mid2 < right) {
        int local_iter = 0;
        total += integral(diff, mid2, right, eps, int_method, &local_iter);
        total_iter += local_iter;
    }
    
    *iter = total_iter;
    return total;
}

/*============================================================================*/
/*                   ДИНАМИЧЕСКОЕ ПОСТРОЕНИЕ ФИГУР                           */
/*============================================================================*/

static int build_figure1(Point points[], int point_count, FigureDescription *figure) {
    int idx_f1f3_left = -1, idx_f1f3_right = -1;
    int idx_f2f3_left = -1, idx_f2f3_right = -1;
    int idx_f1f2 = -1;
    
    int f1f3_indices[MAX_POINTS];
    int f1f3_count = find_all_points_by_type(points, point_count, 2, f1f3_indices);
    
    if (f1f3_count >= 2) {
        for (int i = 0; i < f1f3_count - 1; i++) {
            for (int j = i + 1; j < f1f3_count; j++) {
                if (points[f1f3_indices[j]].x < points[f1f3_indices[i]].x) {
                    int temp = f1f3_indices[i];
                    f1f3_indices[i] = f1f3_indices[j];
                    f1f3_indices[j] = temp;
                }
            }
        }
        idx_f1f3_left = f1f3_indices[0];
        idx_f1f3_right = f1f3_indices[1];
    }
    
    int f2f3_indices[MAX_POINTS];
    int f2f3_count = find_all_points_by_type(points, point_count, 3, f2f3_indices);
    
    if (f2f3_count >= 2) {
        for (int i = 0; i < f2f3_count - 1; i++) {
            for (int j = i + 1; j < f2f3_count; j++) {
                if (points[f2f3_indices[j]].x < points[f2f3_indices[i]].x) {
                    int temp = f2f3_indices[i];
                    f2f3_indices[i] = f2f3_indices[j];
                    f2f3_indices[j] = temp;
                }
            }
        }
        idx_f2f3_left = f2f3_indices[0];
        idx_f2f3_right = f2f3_indices[1];
    }
    
    int f1f2_indices[MAX_POINTS];
    int f1f2_count = find_all_points_by_type(points, point_count, 1, f1f2_indices);
    if (f1f2_count > 0) {
        idx_f1f2 = f1f2_indices[0];
    }
    
    if (idx_f1f3_left == -1 || idx_f1f3_right == -1 || 
        idx_f2f3_left == -1 || idx_f2f3_right == -1 || 
        idx_f1f2 == -1) {
        return -1;
    }
    
    figure->name = "четырёхугольник";
    figure->n_segments = 3;
    
    figure->segments[0].from = idx_f2f3_left;
    figure->segments[0].to = idx_f1f3_left;
    figure->segments[0].upper = FUNC_F1;
    figure->segments[0].lower = FUNC_F3;
    
    figure->segments[1].from = idx_f2f3_left;
    figure->segments[1].to = idx_f1f3_right;
    figure->segments[1].upper = FUNC_F1;
    figure->segments[1].lower = FUNC_F2;
    
    figure->segments[2].from = idx_f1f3_right;
    figure->segments[2].to = idx_f2f3_right;
    figure->segments[2].upper = FUNC_F1;
    figure->segments[2].lower = FUNC_F3;
    
    return 0;
}

static int build_figure2(Point points[], int point_count, FigureDescription *figure) {
    int idx_f1f3_right = -1;
    int idx_f2f3_right = -1;
    int idx_f1f2 = -1;
    
    idx_f1f3_right = find_point_by_type(points, point_count, 2, 1);
    idx_f2f3_right = find_point_by_type(points, point_count, 3, 1);
    
    int f1f2_indices[MAX_POINTS];
    int f1f2_count = find_all_points_by_type(points, point_count, 1, f1f2_indices);
    if (f1f2_count > 0) {
        idx_f1f2 = f1f2_indices[0];
        for (int i = 1; i < f1f2_count; i++) {
            if (points[f1f2_indices[i]].x > points[idx_f1f2].x) {
                idx_f1f2 = f1f2_indices[i];
            }
        }
    }
    
    if (idx_f1f3_right == -1 || idx_f2f3_right == -1 || idx_f1f2 == -1) {
        return -1;
    }
    
    float x_f1f3 = points[idx_f1f3_right].x;
    float x_f2f3 = points[idx_f2f3_right].x;
    float x_f1f2 = points[idx_f1f2].x;
    
    figure->name = "треугольник";
    figure->n_segments = 2;
    
    if (x_f1f3 < x_f2f3 && x_f2f3 < x_f1f2) {
        figure->segments[0].from = idx_f1f3_right;
        figure->segments[0].to = idx_f2f3_right;
        figure->segments[0].upper = FUNC_F1;
        figure->segments[0].lower = FUNC_F3;
        
        figure->segments[1].from = idx_f2f3_right;
        figure->segments[1].to = idx_f1f2;
        figure->segments[1].upper = FUNC_F1;
        figure->segments[1].lower = FUNC_F2;
    } else {
        int left_idx, mid_idx, right_idx;
        
        if (x_f1f3 < x_f2f3 && x_f1f3 < x_f1f2) {
            left_idx = idx_f1f3_right;
            if (x_f2f3 < x_f1f2) {
                mid_idx = idx_f2f3_right;
                right_idx = idx_f1f2;
            } else {
                mid_idx = idx_f1f2;
                right_idx = idx_f2f3_right;
            }
        } else if (x_f2f3 < x_f1f3 && x_f2f3 < x_f1f2) {
            left_idx = idx_f2f3_right;
            if (x_f1f3 < x_f1f2) {
                mid_idx = idx_f1f3_right;
                right_idx = idx_f1f2;
            } else {
                mid_idx = idx_f1f2;
                right_idx = idx_f1f3_right;
            }
        } else {
            left_idx = idx_f1f2;
            if (x_f1f3 < x_f2f3) {
                mid_idx = idx_f1f3_right;
                right_idx = idx_f2f3_right;
            } else {
                mid_idx = idx_f2f3_right;
                right_idx = idx_f1f3_right;
            }
        }
        
        int left_type = points[left_idx].pair_type;
        int mid_type = points[mid_idx].pair_type;
        
        figure->n_segments = 2;
        
        figure->segments[0].from = left_idx;
        figure->segments[0].to = mid_idx;
        figure->segments[1].from = mid_idx;
        figure->segments[1].to = right_idx;
        
        for (int i = 0; i < 2; i++) {
            figure->segments[i].upper = FUNC_F1;
            int type = (i == 0) ? left_type : mid_type;
            if (type == 2) {
                figure->segments[i].lower = FUNC_F3;
            } else {
                figure->segments[i].lower = FUNC_F2;
            }
        }
    }
    
    return 0;
}

/*============================================================================*/
/*                            ОСНОВНЫЕ ФУНКЦИИ                                */
/*============================================================================*/

float calculate_segment_area(Point points[], BoundarySegment *seg, 
                             float eps, int int_method, int *iter) {
    float x1 = points[seg->from].x;
    float x2 = points[seg->to].x;
    
    if (DEBUG_MODE) {
        output_debug_segment(seg->from, seg->to, x1, x2, seg->upper, seg->lower);
    }
    
    float (*diff)(float) = get_diff_function(seg->upper, seg->lower);
    if (!diff) {
        if (DEBUG_MODE) {
            output_debug_error("неизвестная пара функций");
        }
        *iter = 0;
        return 0.0f;
    }
    
    if (seg->lower == FUNC_F3 || seg->upper == FUNC_F3) {
        float area = calculate_area_with_break(x1, x2, diff, eps, int_method, iter);
        if (DEBUG_MODE) {
            output_debug_area(area, *iter);
        }
        return area;
    }
    
    int local_iter = 0;
    float area = integral(diff, x1, x2, eps, int_method, &local_iter);
    *iter = local_iter;
    
    if (DEBUG_MODE) {
        output_debug_area(area, *iter);
    }
    
    return area;
}

float calculate_figure_area(Point points[], const FigureDescription *figure,
                            float eps, int int_method, int *total_iter) {
    float total_area = 0.0f;
    *total_iter = 0;
    
    for (int i = 0; i < figure->n_segments; i++) {
        int iter;
        float area = calculate_segment_area(points, 
                                           (BoundarySegment*)&figure->segments[i],
                                           eps, int_method, &iter);
        total_area += area;
        *total_iter += iter;
    }
    
    return total_area;
}

float calculate_figure1_area(Point points[], int point_count, float eps, 
                            int int_method, int *iterations) {
    FigureDescription figure;
    if (build_figure1(points, point_count, &figure) != 0) {
        printf("ОШИБКА: Не удалось построить фигуру 1\n");
        *iterations = 0;
        return 0.0f;
    }
    return calculate_figure_area(points, &figure, eps, int_method, iterations);
}

float calculate_figure2_area(Point points[], int point_count, float eps, 
                            int int_method, int *iterations) {
    FigureDescription figure;
    if (build_figure2(points, point_count, &figure) != 0) {
        printf("ОШИБКА: Не удалось построить фигуру 2\n");
        *iterations = 0;
        return 0.0f;
    }
    return calculate_figure_area(points, &figure, eps, int_method, iterations);
}