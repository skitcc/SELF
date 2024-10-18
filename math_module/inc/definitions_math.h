#ifndef DEFINITIONS_MATH_H__
#define DEFINITIONS_MATH_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define RADIUS_RESTRICTION 5
#define EPSILON 1e-5
#define ELECTIRC_COEFFICIENT 9e9

#define PIXEL_TO_METER_CONVERSION 1 // Извенить!!!!!

#define OK 0
#define ALLOC_ERR 99

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    unsigned int x;
    unsigned int y;
    double value;
} point_charge;

typedef struct
{
    double x_component;
    double y_component;
} vect;

#ifdef __cplusplus
}
#endif

#endif // DEFINITIONS_MATH_H__
