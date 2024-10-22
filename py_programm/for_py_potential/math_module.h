#ifndef MATH_MODULE_H__
#define MATH_MODULE_H__

#include "definitions_math.h"

#ifdef __cplusplus
extern "C" {
#endif

// Рассчет -- функции на импорт
int calculate_potential_field(int rows, int cols, point_charge *charges, int count_of_charges, double ***field);
int calculate_electric_field(int rows, int cols, point_charge *charges, int count_of_charges, vect ***field);

double calculate_single_point_potential(int x, int y, point_charge *charges, int num_of_charges);

// Вспомогательные функции (трогать только если очень надо)
int is_position_allowed(int x, int y, point_charge charge);

int alloc_potential_field(int rows, int cols, double ***field);
int alloc_electric_field(int rows, int cols, vect ***field);

void free_potential_field(int rows, double **field);  // Ну ладно, эти две можно
void free_electric_field(int rows, vect **field);

double calc_potential(int x, int y, point_charge charge);
vect calc_elecrtic_strenght(int x, int y, point_charge charge);

#ifdef __cplusplus
}
#endif

#endif // MATH_MODULE_H__