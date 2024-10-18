#ifndef DEFINITIONS_MATH_H__
#define DEFINITIONS_MATH_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define RADIUS_RESTRICTION 2
#define EPSILON 1e-5
#define ELECTRIC_COEFFICIENT 9e9

#define PIXEL_TO_METER_CONVERSION 100 // Извенить!!!!!

#define OK 0
#define ALLOC_ERR 99

typedef struct
{
    int x;
    int y;
    double value;
} point_charge;

typedef struct
{
    double x_component;
    double y_component;
} vect;

#endif // DEFINITIONS_MATH_H__
