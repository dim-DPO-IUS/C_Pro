/**
 * @file ui/output.h
 * @brief Функции вывода программы
 */

#ifndef UI_OUTPUT_H
#define UI_OUTPUT_H

#include "../constants.h"
#include "./math/functions.h"
#include <stdio.h>

/*============================================================================*/
/*                            КОНСТАНТЫ ФОРМАТИРОВАНИЯ                       */
/*============================================================================*/

#define FMT_X "%10.6f"
#define FMT_Y "%10.6f"
#define FMT_AREA "%10.6f"
#define FMT_ITER "%d"

/*============================================================================*/
/*                            ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ                        */
/*============================================================================*/

static inline const char* get_pair_string(int pair_type) {
    switch (pair_type) {
        case 1: return "f1-f2";
        case 2: return "f1-f3";
        case 3: return "f2-f3";
        default: return "unknown";
    }
}

static inline const char* get_branch_string(float x, int pair_type) {
    if ((pair_type == 2 || pair_type == 3) && x < 0) {
        return " [левая ветвь f3]";
    }
    if ((pair_type == 2 || pair_type == 3) && x > 0) {
        return " [правая ветвь f3]";
    }
    return "";
}

static inline const char* get_root_method_name(int method) {
    static const char* names[] = {"деление отрезка", "хорд", "касательных", "комбинированный"};
    return (method >= 0 && method <= 3) ? names[method] : "деление отрезка";
}

static inline const char* get_int_method_name(int method) {
    static const char* names[] = {"прямоугольники", "трапеции", "Симпсона"};
    return (method >= 0 && method <= 2) ? names[method] : "трапеции";
}

/*============================================================================*/
/*                            ВЫВОД ЗАГОЛОВКОВ                               */
/*============================================================================*/

static inline void output_main_header(void) {
    printf("\n========================================\n");
    printf("  ВЫЧИСЛЕНИЕ ПЛОЩАДИ ФИГУРЫ\n");
    printf("========================================\n\n");
    printf("Кривые:\n");
    printf("  f1(x) = 0.6x + 3\n");
    printf("  f2(x) = (x-2)^3 - 1\n");
    printf("  f3(x) = 3/x\n\n");
}

static inline void print_separator(void) {
    printf("========================================\n");
}

static inline void print_section_separator(void) {
    printf("-------------------------\n");
}

/*============================================================================*/
/*                            ВЫВОД ТОЧЕК (БАЗОВЫЙ)                          */
/*============================================================================*/

static inline void print_point_line(int num, float x, float y, 
                                    const char* pair_str, 
                                    const char* branch_str,
                                    int has_iter, int iter) {
    if (has_iter) {
        printf("  Точка %d: x = " FMT_X ", y = " FMT_Y " (%s)%s [итераций: " FMT_ITER "]\n", 
               num, x, y, pair_str, branch_str, iter);
    } else {
        printf("  Точка %d: x = " FMT_X ", y = " FMT_Y " (%s)%s\n", 
               num, x, y, pair_str, branch_str);
    }
}

static inline void print_points(Point points[], int count, 
                                int show_iterations, 
                                const char* title) {
    if (title) {
        printf("\n%s\n", title);
    }
    
    for (int i = 0; i < count; i++) {
        const char* pair_str = get_pair_string(points[i].pair_type);
        const char* branch_str = get_branch_string(points[i].x, points[i].pair_type);
        print_point_line(i+1, points[i].x, points[i].y, 
                        pair_str, branch_str, 
                        show_iterations, points[i].iterations);
    }
    
    if (!title) {
        printf("\n");
    }
}

/*============================================================================*/
/*                            ВЫВОД ТОЧЕК (СПЕЦИАЛЬНЫЕ РЕЖИМЫ)               */
/*============================================================================*/

static inline void output_main_points_only(Point points[], int point_count) {
    print_points(points, point_count, 0, "Координаты точек пересечения:");
}

static inline void output_main_points_summary(Point points[], int point_count) {
    printf("Найдено %d точек пересечения\n", point_count);
    print_points(points, point_count, 0, NULL);
}

static inline void output_points_detailed(Point points[], int count) {
    print_points(points, count, 1, "Детальная информация о точках пересечения:");
}

/*============================================================================*/
/*                            ВЫВОД РЕЗУЛЬТАТОВ ТЕСТИРОВАНИЯ                  */
/*============================================================================*/

static inline void output_main_results(float area1, float area2, float total_area, 
                                      int root_method, int int_method, 
                                      int total_root_iterations,
                                      int fig1_iter, int fig2_iter, 
                                      int total_integral_iterations, int print_iterations) {
    printf("\n");
    print_separator();
    printf("РЕЗУЛЬТАТЫ:\n");
    printf("  Фигура 1 (четырёхугольник): " FMT_AREA "\n", area1);
    printf("  Фигура 2 (треугольник): " FMT_AREA "\n", area2);
    printf("  ОБЩАЯ ПЛОЩАДЬ: " FMT_AREA "\n", total_area);
    printf("  Метод поиска корней: %s\n", get_root_method_name(root_method));
    printf("  Метод интегрирования: %s\n", get_int_method_name(int_method));
    print_separator();
    printf("\n");
    
    if (print_iterations) {
        printf("Детализация итераций:\n");
        printf("  Поиск корней: %d итераций\n", total_root_iterations);
        printf("  Интегрирование:\n");
        printf("    Фигура 1: %d итераций\n", fig1_iter);
        printf("    Фигура 2: %d итераций\n", fig2_iter);
        printf("    Всего (интегрирование): %d итераций\n", total_integral_iterations);
        printf("    ОБЩЕЕ ЧИСЛО ИТЕРАЦИЙ: %d\n", total_root_iterations + total_integral_iterations);
        printf("\n");
    }
}

/*============================================================================*/
/*                            ВЫВОД ДЛЯ РЕЖИМА -a                            */
/*============================================================================*/

static inline void output_abscissas_only(Point points[], int point_count) {
    printf("\nАбсциссы точек пересечения:\n");
    printf("---------------------------\n");
    for (int i = 0; i < point_count; i++) {
        printf("  Точка %d: x = " FMT_X "\n", i+1, points[i].x);
    }
    printf("\n");
}

/*============================================================================*/
/*                            ВЫВОД ДЛЯ РЕЖИМА -i                            */
/*============================================================================*/

static inline void output_iterations_full(Point points[], int count,
                                         int root_method) {
    printf("\nМетод поиска точек: %s\n", get_root_method_name(root_method));
    
    printf("\nТаблица точек и итераций\n");
    printf("----------------------------\n");
    int total_point_iter = 0;
    for (int i = 0; i < count; i++) {
        const char* pair_str = get_pair_string(points[i].pair_type);
        printf("  Точка %d: x = " FMT_X ", y = " FMT_Y " (%s) [итераций: %d]\n", 
               i+1, points[i].x, points[i].y, pair_str, points[i].iterations);
        total_point_iter += points[i].iterations;
    }
    
    printf("\nВСЕГО ИТЕРАЦИЙ НА ПОИСК ТОЧЕК: %d\n", total_point_iter);
    printf("\n");
}

/*============================================================================*/
/*                            ВЫВОД ОТЛАДОЧНОЙ ИНФОРМАЦИИ                    */
/*============================================================================*/

static inline void output_debug_segment(int from, int to, float x1, float x2, 
                                       int upper, int lower) {
    printf("  Сегмент [%d->%d] от %f до %f: верх=f%d, низ=f%d\n", 
           from, to, x1, x2, upper, lower);
}

static inline void output_debug_area(float area, int iter) {
    printf("    площадь = %f, итераций = %d\n", area, iter);
}

static inline void output_debug_error(const char* msg) {
    printf("    ОШИБКА: %s\n", msg);
}

/*============================================================================*/
/*                            ВЫВОД ТЕСТОВ                                   */
/*============================================================================*/

static inline void output_test_full_header(float eps1, float eps2) {
    printf("\n========================================\n");
    printf("   ПОЛНОЕ ТЕСТИРОВАНИЕ ПРОГРАММЫ\n");
    printf("========================================\n");
    printf("Точность для корней: %e\n", eps1);
    printf("Точность для интегралов: %e\n\n", eps2);
}

static inline void output_test_roots_header(float eps) {
    printf("\n========================================================================\n");
    printf("   ТЕСТ 1: ПОИСК КОРНЕЙ ВСЕМИ МЕТОДАМИ (точность = %e)\n", eps);
    printf("========================================================================\n");
}

static inline void output_test_roots_table_header(void) {
    printf("\n");
    printf("==========================================================\n");
    printf("| Корни | Точность   | Итераций   | Метод                |\n");
    printf("|-------|------------|------------|----------------------|\n");
}

static inline void output_test_roots_row(int roots, float accuracy, int iterations, const char* method) {
    printf("|   %d   |  %.2e  | %d         | %-22s\t |\n",
           roots, accuracy, iterations, method);
}

static inline void output_test_roots_table_footer(void) {
    printf("==========================================================\n\n");
}

static inline void output_test_integrals_header(float eps) {
    printf("\n========================================================================\n");
    printf("   ТЕСТ 2: ИНТЕГРИРОВАНИЕ ВСЕМИ МЕТОДАМИ (точность = %e)\n", eps);
    printf("========================================================================\n");
}

static inline void output_test_integrals_table_header(void) {
    printf("\n");
    printf("===================================================\n");
    printf("| Фигура | Метод          | Площадь    | Итерации |\n");
    printf("|--------|----------------|------------|----------|\n");
}

static inline void output_test_integrals_row_figure1(int method_index, const char* method_name, 
                                                     float area, int iter) {
    if (method_index == 0) {
        printf("| Фиг.1  | %-22s | " FMT_AREA " | %5d    |\n", method_name, area, iter);
    } else {
        printf("|        | %-22s | " FMT_AREA " | %5d    |\n", method_name, area, iter);
    }
}

static inline void output_test_integrals_row_figure2(int method_index, const char* method_name, 
                                                     float area, int iter) {
    if (method_index == 0) {
        printf("| Фиг.2  | %-22s | " FMT_AREA " | %5d    |\n", method_name, area, iter);
    } else {
        printf("|        | %-22s | " FMT_AREA " | %5d    |\n", method_name, area, iter);
    }
}

static inline void output_test_integrals_separator(void) {
    printf("|--------|----------------|------------|----------|\n");
}

static inline void output_test_integrals_table_footer(void) {
    printf("===================================================\n\n");
}

/*============================================================================*/
/*                            ВЫВОД ВИЗУАЛИЗАЦИИ                             */
/*============================================================================*/

static inline void output_vis_data_generated(void) {
    printf("Сгенерированы файлы с данными\n");
}

static inline void output_vis_script_created(const char* script_name) {
    printf("Создан скрипт %s\n", script_name);
}

static inline void output_vis_launching(void) {
    printf("Запуск GNUplot...\n");
    printf("Должно открыться окно с графиком.\n");
    printf("После просмотра закройте окно и нажмите Enter в этом терминале.\n\n");
}

static inline void output_vis_second_plot(void) {
    printf("\nПервый график закрыт. Запускаем второй график с заливкой...\n");
}

static inline void output_vis_complete(void) {
    printf("\nВизуализация завершена.\n");
    printf("Графики сохранены в файлы:\n");
    printf("  - curves.png (кривые и точки)\n");
    printf("  - filled_area.png (заштрихованная площадь)\n");
}

/*============================================================================*/
/*                            ВЫВОД HELP И ЛОГА                              */
/*============================================================================*/

static inline void output_help(void) {
    printf("\n========================================\n");
    printf("  ПРОГРАММА ВЫЧИСЛЕНИЯ ПЛОЩАДИ ФИГУРЫ\n");
    printf("========================================\n\n");
    printf("Использование: ./area [ОПЦИИ]\n\n");
    printf("ОПЦИИ:\n");
    printf("  -h, --help         Показать эту справку\n");
    printf("  -a, --abscissas    Вывести только абсциссы точек пересечения\n");
    printf("  -p, --points       Вывести подробную информацию о точках\n");
    printf("  -i, --iterations   Вывести число итераций\n");
    printf("  -t eps1 eps2       Режим тестирования\n");
    printf("  -v, --visualize    Визуализировать результат (GNUplot)\n");
    printf("  --root-method M    Выбрать метод поиска корней (0-3)\n");
    printf("                     0 - деление отрезка\n");
    printf("                     1 - метод хорд\n");
    printf("                     2 - метод касательных\n");
    printf("                     3 - комбинированный\n");
    printf("  --int-method M     Выбрать метод интегрирования (0-2)\n");
    printf("                     0 - прямоугольники\n");
    printf("                     1 - трапеции\n");
    printf("                     2 - Симпсон\n");
    printf("  --log              Сохранить результаты в файл\n");
    printf("\nПРИОРИТЕТЫ:\n");
    printf("  -p важнее -a (если указаны оба, выводятся подробные точки)\n");
    printf("\nПримеры:\n");
    printf("  ./area -a              (только абсциссы)\n");
    printf("  ./area -p              (подробные точки)\n");
    printf("  ./area -i              (полный вывод с итерациями)\n");
    printf("  ./area -v --int-method 2\n");
    printf("  ./area -t 1e-6 1e-4    (тестирование)\n\n");
}

static inline void output_log_saved(void) {
    printf("Результаты сохранены в results.log\n");
}

/*============================================================================*/
/*                            ВЫВОД ТАБЛИЦЫ СРАВНЕНИЯ МЕТОДОВ                */
/*============================================================================*/

/**
 * @brief Вывод таблицы сравнения методов поиска корней
 * @param methods Массив названий методов (4 элемента)
 * @param f1f3_l Массив координат для f1-f3 левая (4 элемента)
 * @param f2f3_l Массив координат для f2-f3 левая (4 элемента)
 * @param f1f3_r Массив координат для f1-f3 правая (4 элемента)
 * @param f2f3_r Массив координат для f2-f3 правая (4 элемента)
 * @param f1f2 Массив координат для f1-f2 (4 элемента)
 * @param iterations Массив чисел итераций (4 элемента)
 */
static inline void output_methods_comparison_table(const char* methods[],
                                                  float f1f3_l[], float f2f3_l[],
                                                  float f1f3_r[], float f2f3_r[],
                                                  float f1f2[]) {
    printf("\n=== КООРДИНАТЫ ТОЧЕК ПЕРЕСЕЧЕНИЯ ПО МЕТОДАМ ===\n\n");
    printf("----------------------------------------------------------------------------------\n");
    printf("| f₁-f₃ (л) | f₂-f₃ (л) | f₁-f₃ (п) | f₂-f₃ (п) |    f₁-f₂  | Метод              |\n");
    printf("|-----------|-----------|-----------|-----------|-----------|--------------------|\n");
    
    for (int i = 0; i < 4; i++) {
        printf("|"FMT_X" |"FMT_X" |"FMT_X" |"FMT_X" |"FMT_X" | %-20s\t |\n",
               f1f3_l[i], f2f3_l[i], f1f3_r[i], f2f3_r[i], f1f2[i], methods[i]);
        
        if (i < 3) {
            printf("|-----------|-----------|-----------|-----------|-----------|--------------------|\n");
        }
    }
    
    printf("----------------------------------------------------------------------------------\n\n");
}

#endif /* UI_OUTPUT_H */