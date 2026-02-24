/**
 * @file ui/utils.c
 * @brief Реализация вспомогательных функций
 */

#include "ui/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

/*============================================================================*/
/*                            БАЗОВЫЕ УТИЛИТЫ                                 */
/*============================================================================*/

int utils_is_valid(float x) {
    return !isnan(x) && !isinf(x);
}

/*============================================================================*/
/*                            ЛОГИРОВАНИЕ                                    */
/*============================================================================*/

void utils_log_results(float x_points[], int n_points, float area, int iterations[]) {
    time_t now;
    time(&now);
    
    FILE *log = fopen("results.log", "a");
    if (!log) return;
    
    fprintf(log, "=== %s", ctime(&now));
    fprintf(log, "Точки пересечения:\n");
    for (int i = 0; i < n_points; i++) {
        fprintf(log, "  x%d = %f\n", i + 1, x_points[i]);
    }
    fprintf(log, "Площадь фигуры: %f\n", area);
    if (iterations) {
        fprintf(log, "Итерации (корни): %d\n", iterations[0]);
        fprintf(log, "Итерации (интегрирование): %d\n", iterations[1]);
    }
    fprintf(log, "========================\n\n");
    
    fclose(log);
    output_log_saved();
}

/*============================================================================*/
/*                            СПРАВКА И ПАРСИНГ                              */
/*============================================================================*/

void utils_print_help(void) {
    output_help();
}

int utils_parse_args(int argc, char *argv[], Config *cfg) {
    cfg->eps_root = EPS_ROOT_DEFAULT;
    cfg->eps_integral = EPS_INTEGRAL_DEFAULT;
    cfg->root_method = ROOT_METHOD_DEFAULT;
    cfg->integral_method = INTEGRAL_METHOD_DEFAULT;
    
    cfg->test_mode = 0;
    cfg->visualize = 0;
    
    cfg->flag_abscissas = 0;
    cfg->flag_points = 0;
    cfg->flag_iterations = 0;
    cfg->flag_log = 0;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            utils_print_help();
            return 1;
        }
        else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--abscissas") == 0) {
            cfg->flag_abscissas = 1;
        }
        else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--points") == 0) {
            cfg->flag_points = 1;
        }
        else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--iterations") == 0) {
            cfg->flag_iterations = 1;
        }
        else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--visualize") == 0) {
            cfg->visualize = 1;
        }
        else if (strcmp(argv[i], "--log") == 0) {
            cfg->flag_log = 1;
        }
        else if (strcmp(argv[i], "--root-method") == 0 && i + 1 < argc) {
            cfg->root_method = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--int-method") == 0 && i + 1 < argc) {
            cfg->integral_method = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-t") == 0) {
            cfg->test_mode = 1;
            if (i + 2 < argc && argv[i+1][0] != '-') {
                cfg->eps_root = atof(argv[++i]);
                cfg->eps_integral = atof(argv[++i]);
            } else if (i + 1 < argc && argv[i+1][0] != '-') {
                cfg->eps_root = atof(argv[++i]);
            }
        }
    }
    
    return 0;
}