/**
 * @file ui/test_runner.c
 * @brief Реализация тестовых функций
 */

#include "ui/test_runner.h"
#include <stdio.h>
#include <math.h>

/*============================================================================*/
/*                            ТЕСТОВЫЕ ФУНКЦИИ                               */
/*============================================================================*/

/**
 * @brief Тестовая функция f(x) = x²
 */
static float test_square(float x) {
    return x * x;
}

/**
 * @brief Тестовая константная функция f(x) = 4
 */
static float test_const4(float x) {
    (void)x;
    return TEST_CONSTANT_VALUE;
}

static float test_zero(float x) {
    (void)x;
    return 0.0f;
}

static float test_sin(float x) {
    return sinf(x);
}


/*============================================================================*/
/*                    ТЕСТ 1: ПОИСК КОРНЕЙ ВСЕМИ МЕТОДАМИ                     */
/*============================================================================*/

void test_roots_all_methods(float eps) {
    output_test_roots_header(eps);
    
    float xmin = SEARCH_LEFT, xmax = SEARCH_RIGHT;
    int steps = SEARCH_STEPS;
    
    const char* method_names[] = {
        "Деление отрезка",
        "Метод хорд",
        "Метод касательных",
        "Комбинированный"
    };
    
    Point points[4][MAX_POINTS];
    int counts[4];
    
    for (int method = 0; method < 4; method++) {
        counts[method] = find_all_intersections(xmin, xmax, steps, eps,
                                                points[method], MAX_POINTS, method);
        sort_points(points[method], counts[method]);
    }
    
    output_test_roots_table_header();
    
    for (int m = 0; m < 4; m++) {
        float avg_error = 0.0f;
        int matched = 0;
        
        for (int i = 0; i < counts[0]; i++) {
            float x_etalon = points[0][i].x;
            float min_error = 1e10f;
            
            for (int j = 0; j < counts[m]; j++) {
                float error = fabsf(points[m][j].x - x_etalon);
                if (error < min_error) min_error = error;
            }
            
            if (min_error < TEST_MATCH_THRESHOLD) {
                avg_error += min_error;
                matched++;
            }
        }
        
        if (matched > 0) avg_error /= matched;
        
        int total_iter = 0;
        for (int j = 0; j < counts[m]; j++) total_iter += points[m][j].iterations;
        
        output_test_roots_row(counts[m], avg_error, total_iter, method_names[m]);
    }
    
    output_test_roots_table_footer();
}

/*============================================================================*/
/*               ТЕСТ 2: ИНТЕГРИРОВАНИЕ ВСЕМИ МЕТОДАМИ                      */
/*============================================================================*/

void test_integral_all_methods(float eps) {
    output_test_integrals_header(eps);
        
    Point points[MAX_POINTS];
    int point_count = find_all_intersections(SEARCH_LEFT, SEARCH_RIGHT, SEARCH_STEPS,
                                             EPS_ROOT_DEFAULT, points, MAX_POINTS, 0);
    sort_points(points, point_count);
    
    const char* method_names[] = {
        "Прямоугольники",
        "Трапеции",
        "Симпсона"
    };
    
    output_test_integrals_table_header();
    
    for (int m = 0; m < 3; m++) {
        int iter;
        float area = calculate_figure1_area(points, point_count, eps, m, &iter);
        output_test_integrals_row_figure1(m, method_names[m], area, iter);
    }
    
    output_test_integrals_separator();
    
    for (int m = 0; m < 3; m++) {
        int iter;
        float area = calculate_figure2_area(points, point_count, eps, m, &iter);
        output_test_integrals_row_figure2(m, method_names[m], area, iter);
    }
    
    output_test_integrals_table_footer();
}

/*============================================================================*/
/*                            ТЕСТИРОВАНИЕ ПОИСКА КОРНЕЙ                     */
/*============================================================================*/

void test_roots(float eps) {
    printf("\n--- ТЕСТИРОВАНИЕ ПОИСКА КОРНЕЙ (учебные примеры) ---\n");
    printf("Точность: %e\n\n", eps);
    
    int iter;
    float root = root_find_div(TEST_ROOT_A, TEST_ROOT_B, eps, 
                              test_square, test_const4, &iter);
    printf("x² = 4 на [%g, %g]: x = %f (итераций: %d)\n", 
           TEST_ROOT_A, TEST_ROOT_B, root, iter);
    
    root = root_find_chord(TEST_ROOT_B, TEST_ROOT_C, eps, 
                          test_sin, test_zero, &iter);
    printf("sin(x) = 0 на [%g, %g]: x = %f (итераций: %d)\n", 
           TEST_ROOT_B, TEST_ROOT_C, root, iter);
}

/*============================================================================*/
/*                            ТЕСТИРОВАНИЕ ИНТЕГРИРОВАНИЯ                    */
/*============================================================================*/

void test_integrals(float eps) {
    printf("\n--- ТЕСТИРОВАНИЕ ИНТЕГРИРОВАНИЯ (учебные примеры) ---\n");
    printf("Точность: %e\n\n", eps);
    
    int iter;
    float result = integral_trapezoid(TEST_INTEGRAL_A, TEST_INTEGRAL_B, eps, 
                                     test_square, &iter);
    printf("∫x² dx от %g до %g = %f (итераций: %d)\n", 
           TEST_INTEGRAL_A, TEST_INTEGRAL_B, result, iter);
    
    result = integral_simpson(TEST_INTEGRAL_A, (float)M_PI, eps, 
                             test_sin, &iter);
    printf("∫sin(x) dx от %g до π = %f (итераций: %d)\n", 
           TEST_INTEGRAL_A, result, iter);
}

/*============================================================================*/
/*                            ЗАПУСК ВСЕХ ТЕСТОВ                             */
/*============================================================================*/

void run_all_tests(float eps1, float eps2) {
    output_test_full_header(eps1, eps2);
    
    test_roots_all_methods(eps1);
    test_integral_all_methods(eps2);
        
    printf("===================================================\n\n");
}