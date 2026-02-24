/**
 * @file main.c
 * @brief Главный файл программы вычисления площади фигур
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#include "constants.h"
#include "math/functions.h"
#include "math/roots.h"
#include "math/integrals.h"
#include "geometry/pointfinder.h"
#include "geometry/area_calculator.h"
#include "ui/test_runner.h"
#include "ui/visualization.h"
#include "ui/utils.h"
#include "ui/output.h"

/*============================================================================*/
/*                            ОСНОВНАЯ ФУНКЦИЯ                                */
/*============================================================================*/

int main(int argc, char *argv[]) {
    Config cfg;
    int parse_result = utils_parse_args(argc, argv, &cfg);
    
    if (parse_result == 1) {
        return 0;
    }
    
    /* Режим тестирования */
    if (cfg.test_mode) {
        run_all_tests(cfg.eps_root, cfg.eps_integral);
        return 0;
    }
    
    /* Поиск точек пересечения */
    Point points[MAX_POINTS];
    int point_count = find_all_intersections(SEARCH_LEFT, SEARCH_RIGHT, 
                                             SEARCH_STEPS, cfg.eps_root,
                                             points, MAX_POINTS,
                                             cfg.root_method);
    
    if (point_count == 0) {
        printf("ОШИБКА: Не найдено ни одной точки пересечения!\n");
        return 1;
    }
    
    sort_points(points, point_count);
    
    /* ПРИОРИТЕТЫ ВЫВОДА:
     * 1. Если есть -p (подробные точки) - выводим их и завершаем
     * 2. Иначе если есть -a (только абсциссы) - выводим их и завершаем
     * 3. Иначе если есть -i (только итерации) - выводим и завершаем
     * 4. Иначе - полный вывод
     */
    
    /* Приоритет 1: подробные точки (-p) */
    if (cfg.flag_points) {
        output_main_points_only(points, point_count);
        return 0;
    }
    
    /* Приоритет 2: только абсциссы (-a) */
    if (cfg.flag_abscissas) {
        // for (int i = 0; i < point_count; i++) {
        //     printf("Точка %d: x = %f\n", i+1, points[i].x);
        // }
        output_abscissas_only(points, point_count);
        return 0;
    }
    
    /* Приоритет 3: только итерации на поиск точек (-i) */
    if (cfg.flag_iterations) {
        output_iterations_full(points, point_count, cfg.root_method);
        return 0;
    }
    
    /* Приоритет 4: полный вывод */
    output_main_header();
    output_main_points_summary(points, point_count);
    
    /* Подготовка массива x-координат для лога */
    float x_points[MAX_POINTS];
    for (int i = 0; i < point_count; i++) {
        x_points[i] = points[i].x;
    }
    
    /* Подсчёт итераций для поиска корней (для лога) */
    int total_root_iterations = 0;
    for (int i = 0; i < point_count; i++) {
        total_root_iterations += points[i].iterations;
    }
    
    /* Вычисление площадей */
    int fig1_iter = 0, fig2_iter = 0;
    float area1 = calculate_figure1_area(points, point_count, cfg.eps_integral, 
                                        cfg.integral_method, &fig1_iter);
    float area2 = calculate_figure2_area(points, point_count, cfg.eps_integral, 
                                        cfg.integral_method, &fig2_iter);
    
    float total_area = area1 + area2;
    int total_integral_iterations = fig1_iter + fig2_iter;
    
    /* Вывод результатов */
    output_main_results(area1, area2, total_area, 
                    cfg.root_method,              
                    cfg.integral_method, 
                    total_root_iterations,
                    fig1_iter, fig2_iter, 
                    total_integral_iterations, 
                    0); // print_iterations = 0 для обычного режима
    
    /* Сохранение в лог */
    if (cfg.flag_log) {
        int iterations[4] = {total_root_iterations, fig1_iter, fig2_iter, total_integral_iterations};
        utils_log_results(x_points, point_count, total_area, iterations);
    }
    
    /* Визуализация */
    if (cfg.visualize) {
        visualize(points, point_count, total_area);
        vis_cleanup();
    }
    
    return 0;
}