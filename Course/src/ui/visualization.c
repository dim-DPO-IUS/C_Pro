/**
 * @file ui/visualization.c
 * @brief Реализация визуализации результатов
 */

#include "ui/visualization.h"
#include <stdio.h>
#include <stdlib.h>

/*============================================================================*/
/*                            ГЕНЕРАЦИЯ ДАННЫХ ФУНКЦИЙ                       */
/*============================================================================*/

void vis_generate_data(float x_min, float x_max, float step) {
    FILE *f1_data = fopen("f1.dat", "w");
    FILE *f2_data = fopen("f2.dat", "w");
    FILE *f3_data = fopen("f3.dat", "w");
    
    if (!f1_data || !f2_data || !f3_data) {
        printf("Ошибка создания файлов данных\n");
        if (f1_data) fclose(f1_data);
        if (f2_data) fclose(f2_data);
        if (f3_data) fclose(f3_data);
        return;
    }
    
    for (float x = x_min; x <= x_max; x += step) {
        fprintf(f1_data, "%f %f\n", x, f1(x));
        fprintf(f2_data, "%f %f\n", x, f2(x));
        fprintf(f3_data, "%f %f\n", x, f3(x));
    }
    
    fclose(f1_data);
    fclose(f2_data);
    fclose(f3_data);
    
    output_vis_data_generated();
}

/*============================================================================*/
/*                            ГРАФИК С ТОЧКАМИ ПЕРЕСЕЧЕНИЯ                   */
/*============================================================================*/

void vis_create_points_plot(Point points[], int n_points, float area) {
    FILE *script = fopen("plot.gp", "w");
    if (!script) return;
    
    fprintf(script, "set terminal qt persist\n");
    fprintf(script, "set title 'Кривые и точки пересечения (Общая площадь = %.6f)'\n", area);
    fprintf(script, "set xlabel 'x'\n");
    fprintf(script, "set ylabel 'y'\n");
    fprintf(script, "set yrange [%f:%f]\n", Y_MIN, Y_MAX);
    fprintf(script, "set grid\n");
    fprintf(script, "set key outside\n");
    
    fprintf(script, "plot 'f1.dat' with lines linewidth %d title 'f1(x)=0.6x+3', \\\n", VISUAL_LINE_WIDTH);
    fprintf(script, "     'f2.dat' with lines linewidth %d title 'f2(x)=(x-2)^3-1', \\\n", VISUAL_LINE_WIDTH);
    fprintf(script, "     'f3.dat' with lines linewidth %d title 'f3(x)=3/x', \\\n", VISUAL_LINE_WIDTH);
    fprintf(script, "     '-' with points pt 7 ps %d title 'Точки пересечения'\n", VISUAL_POINT_SIZE);
    
    for (int i = 0; i < n_points; i++) {
        fprintf(script, "%f %f\n", points[i].x, points[i].y);
    }
    fprintf(script, "e\n");
    
    fprintf(script, "set terminal pngcairo size 1024,768\n");
    fprintf(script, "set output 'curves.png'\n");
    fprintf(script, "replot\n");
    fprintf(script, "set terminal qt persist\n");
    fprintf(script, "set output\n");
    fprintf(script, "pause -1 'Нажмите Enter для продолжения...'\n");
    
    fclose(script);
    
    output_vis_script_created("plot.gp");
}

/*============================================================================*/
/*                            ГРАФИК С ЗАЛИВКОЙ ФИГУР                        */
/*============================================================================*/

void vis_create_filled_plot(Point points[], int n_points, float area) {
    FILE *script = fopen("filled.gp", "w");
    if (!script) return;
    
    fprintf(script, "set terminal qt persist\n");
    fprintf(script, "set title 'Площадь фигуры (Общая площадь = %.6f)'\n", area);
    fprintf(script, "set xlabel 'x'\n");
    fprintf(script, "set ylabel 'y'\n");
    fprintf(script, "set yrange [%f:%f]\n", Y_MIN, Y_MAX);
    fprintf(script, "set grid\n");
    fprintf(script, "set key outside\n");
    
    // Фигура 1
    FILE *fig1_data = fopen("fig1.dat", "w");
    if (fig1_data) {
        float step = VISUAL_STEP_SIZE;
        
        for (float x = points[1].x; x >= points[0].x; x -= step) {
            fprintf(fig1_data, "%f %f\n", x, f3(x));
        }
        
        for (float x = points[0].x; x <= points[2].x; x += step) {
            fprintf(fig1_data, "%f %f\n", x, f1(x));
        }
        
        for (float x = points[2].x; x <= points[3].x; x += step) {
            fprintf(fig1_data, "%f %f\n", x, f3(x));
        }
        
        for (float x = points[3].x; x >= points[1].x; x -= step) {
            fprintf(fig1_data, "%f %f\n", x, f2(x));
        }
        
        fclose(fig1_data);
    }
    
    // Фигура 2
    FILE *fig2_data = fopen("fig2.dat", "w");
    if (fig2_data) {
        float step = VISUAL_STEP_SIZE;
        
        for (float x = points[2].x; x <= points[4].x; x += step) {
            fprintf(fig2_data, "%f %f\n", x, f1(x));
        }
        
        for (float x = points[4].x; x >= points[3].x; x -= step) {
            fprintf(fig2_data, "%f %f\n", x, f2(x));
        }
        
        for (float x = points[3].x; x >= points[2].x; x -= step) {
            fprintf(fig2_data, "%f %f\n", x, f3(x));
        }
        
        fclose(fig2_data);
    }
    
    // Точки пересечения
    FILE *pts_data = fopen("points.dat", "w");
    if (pts_data) {
        for (int i = 0; i < n_points; i++) {
            fprintf(pts_data, "%f %f\n", points[i].x, points[i].y);
        }
        fclose(pts_data);
    }
    
    fprintf(script, "plot ");
    fprintf(script, "'fig1.dat' with filledcurves closed title 'Фигура 1' lc rgb '#90EE90', \\\n");
    fprintf(script, "'fig2.dat' with filledcurves closed title 'Фигура 2' lc rgb '#FFA500', \\\n");
    fprintf(script, "'f1.dat' with lines linewidth %d title 'f1(x)=0.6x+3', \\\n", VISUAL_LINE_WIDTH);
    fprintf(script, "'f2.dat' with lines linewidth %d title 'f2(x)=(x-2)^3-1', \\\n", VISUAL_LINE_WIDTH);
    fprintf(script, "'f3.dat' with lines linewidth %d title 'f3(x)=3/x', \\\n", VISUAL_LINE_WIDTH);
    fprintf(script, "'points.dat' with points pt 7 ps %d title 'Точки пересечения'\n", VISUAL_POINT_SIZE);
    
    fprintf(script, "set terminal pngcairo size 1024,768\n");
    fprintf(script, "set output 'filled_area.png'\n");
    fprintf(script, "replot\n");
    fprintf(script, "set terminal qt persist\n");
    fprintf(script, "set output\n");
    fprintf(script, "pause -1 'Нажмите Enter для продолжения...'\n");
    
    fclose(script);
    
    output_vis_script_created("filled.gp");
}

/*============================================================================*/
/*                            ЗАПУСК GNUPLOT                                 */
/*============================================================================*/

void vis_show_plots(void) {
    int ret;
    
    output_vis_launching();
    
    ret = system("gnuplot -persist plot.gp 2>/dev/null");
    if (ret != 0) {
        ret = system("gnuplot-qt -persist plot.gp 2>/dev/null");
    }
    (void)ret;
    
    output_vis_second_plot();
    
    ret = system("gnuplot -persist filled.gp 2>/dev/null");
    if (ret != 0) {
        ret = system("gnuplot-qt -persist filled.gp 2>/dev/null");
    }
    (void)ret;
    
    output_vis_complete();
}

/*============================================================================*/
/*                            ОЧИСТКА ВРЕМЕННЫХ ФАЙЛОВ                       */
/*============================================================================*/

void vis_cleanup(void) {
    int ret = system("rm -f f1.dat f2.dat f3.dat plot.gp filled.gp points.dat fig1.dat fig2.dat 2>/dev/null");
    (void)ret;
}

/*============================================================================*/
/*                            ПОЛНАЯ ВИЗУАЛИЗАЦИЯ                            */
/*============================================================================*/

void visualize(Point points[], int n_points, float area) {
    printf("\n=== ВИЗУАЛИЗАЦИЯ ===\n");
    
    float x_min = points[0].x - VISUAL_MARGIN;
    float x_max = points[n_points - 1].x + VISUAL_MARGIN;
    float step = (x_max - x_min) / VISUAL_STEPS;
    
    vis_generate_data(x_min, x_max, step);
    vis_create_points_plot(points, n_points, area);
    vis_create_filled_plot(points, n_points, area);
    
    vis_show_plots();
}