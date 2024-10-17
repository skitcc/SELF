#include "math_module.h"

// Расчет полей ////////////////////////////////////////////////////////////////////////////////////////////////
int calculate_potential_field(int rows, int cols, point_charge *charges, int count_of_charges, double ***field)
{
    if (alloc_potential_field(rows, cols, field) != OK)
        return ALLOC_ERR

    for (int c = 0; c < count_of_charges; c++)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (is_position_allowed(i, j, charges[c]))
                {
                    *field[i][j] = calc_potential(i, j, charges[c]);
                }
            }
        }
    }

    return OK;
}

int calculate_electric_field(int rows, int cols, point_charge *charges, int count_of_charges, vect ***field)
{
    if (alloc_electric_field(rows, cols, field) != OK)
        return ALLOC_ERR

    for (int c = 0; c < count_of_charges; c++)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (is_position_allowed(i, j, charges[c]))
                {
                    *field[i][j] = calc_electric_field(i, j, charges[c]);
                }
            }
        }
    }

    return OK;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Проверка позиции ///////////////////////////////////////////////////////////
int is_position_allowed(int x, int y, point_charge charge)
{
    double r = sqrt(pow(fabs(charge.x - x), 2) + pow(fabs(charge.y - y), 2));
    return ((r - RADIUS_RESTRICTION < EPSILON) ? 1 : 0);
}
//////////////////////////////////////////////////////////////////////////////
// Выделение памяти ///////////////////////////////////////////////
int alloc_potential_field(int rows, int cols, double ***field)
{
    *field = (double **)calloc(rows, sizeof(double **));
    if (*field == NULL)
        return ALLOC_ERR;

    for (int i = 0; i < rows; i++)
    {
        (*field)[i] = (double *)calloc(cols, sizeof(double));
        if ((*field)[i] == NULL)
        {
            free_potential_field(field);
            return ALLOC_ERR;
        }
    }

    return OK;
}

int alloc_electric_field(int rows, int cols, vect ***field)
{
    *field = (vect **)calloc(rows, sizeof(vect **));
    if (*field == NULL)
        return ALLOC_ERR;
    
    for (int i = 0; i < rows; i++)
    {
        (*field)[i] = (vect *)calloc(cols, sizeof(vect));
        if ((*field)[i] == NULL)
        {
            free_electric_field(field);
            return ALLOC_ERR;
        }
    }
}
//////////////////////////////////////////////////////////////////
// Освобождение памяти //////////////////////////////
void free_potential_field(int rows, double ***field)
{
    for (int i = 0; i < rows; i++)
    {
        free(*field[i]);
    }
    free(*field);
}

void free_electric_field(int rows, vect ***field)
{
    for (int i = 0; i < rows; i++)
    {
        free(*field[i]);
    }
    free(*field);
}
/////////////////////////////////////////////////////
// Расчет значений /////////////////////////////////////////////////////////////////////////////////////////
double calc_potential(int x, int y, point_charge charge)
{
    double r = PIXEL_TO_METER_CONVERSION * sqrt(pow(fabs(charge.x - x), 2) + pow(fabs(charge.y - y), 2));
    return (ELECTRIC_COEFFICIENT * charge.value / r);
}

vect calc_elecrtic_strenght(int x, int y, point_charge charge)
{
    double r = sqrt(pow(fabs(charge.x - x), 2) + pow(fabs(charge.y - y), 2));
    double full_vect = ELECTRIC_COEFFICIENT * charge.value / pow(r, 2);
    vect result;
    result.x_component = cos(x - charge.x) * full_vect;
    result.y_component = sin(y - charge.y) * full_vect;
    return result;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////