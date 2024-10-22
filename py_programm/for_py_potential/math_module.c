#include "math_module.h"

// Расчет потенциала единственной точки //////////////////////////////////////////////////////////
double calculate_single_point_potential(int x, int y, point_charge *charges, int num_of_charges)
{
    double potential = 0.0;
    for (int c = 0; c < num_of_charges; c++)
    {
        if (!is_position_allowed(x, y, charges[c]))
            return 0.0;
        potential += calc_potential(x, y, charges[c]);
    }
    return potential;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// Расчет полей ////////////////////////////////////////////////////////////////////////////////////////////////
int calculate_potential_field(int rows, int cols, point_charge *charges, int count_of_charges, double ***field)
{
    if (alloc_potential_field(rows, cols, field) != OK)
        return ALLOC_ERR;

    for (int c = 0; c < count_of_charges; c++)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (is_position_allowed(i, j, charges[c]))
                {
                    (*field)[i][j] += calc_potential(i, j, charges[c]);
                }
            }
        }
    }
    for (int c = 0; c < count_of_charges; c++)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (!is_position_allowed(i, j, charges[c]))
                {
                    (*field)[i][j] = 0.0;
                }
            }
        }
    }

    return OK;
}

int calculate_electric_field(int rows, int cols, point_charge *charges, int count_of_charges, vect ***field)
{
    if (alloc_electric_field(rows, cols, field) != OK)
        return ALLOC_ERR;

    for (int c = 0; c < count_of_charges; c++)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (is_position_allowed(i, j, charges[c]))
                {
                    vect cc = calc_elecrtic_strenght(i, j, charges[c]);
                    (*field)[i][j].x_component += cc.x_component;
                    (*field)[i][j].y_component += cc.y_component;
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
    double r = sqrt(pow(abs(charge.x - x), 2) + pow(abs(charge.y - y), 2));
    return ((r < RADIUS_RESTRICTION) ? 0 : 1);
}
//////////////////////////////////////////////////////////////////////////////
// Выделение памяти ///////////////////////////////////////////////
int alloc_potential_field(int rows, int cols, double ***field)
{
    double **f = NULL;
    f = (double **)calloc(rows, sizeof(double *));
    if (f == NULL)
        return ALLOC_ERR;

    for (int i = 0; i < rows; i++)
    {
        f[i] = (double *)calloc(cols, sizeof(double));
        if (f[i] == NULL)
        {
            free_potential_field(rows, f);
            return ALLOC_ERR;
        }
    }

    *field = f;

    return OK;
}

int alloc_electric_field(int rows, int cols, vect ***field)
{
    vect **f = NULL;
    f = (vect **)calloc(rows, sizeof(vect *));
    if (f == NULL)
        return ALLOC_ERR;
    
    for (int i = 0; i < rows; i++)
    {
        f[i] = (vect *)calloc(cols, sizeof(vect));
        if (f[i] == NULL)
        {
            free_electric_field(rows, f);
            return ALLOC_ERR;
        }
    }

    *field = f;

    return OK;
}
//////////////////////////////////////////////////////////////////
// Освобождение памяти //////////////////////////////
void free_potential_field(int rows, double **field)
{
    for (int i = 0; i < rows; i++)
    {
        free(field[i]);
    }
    free(field);
}

void free_electric_field(int rows, vect **field)
{
    for (int i = 0; i < rows; i++)
    {
        free(field[i]);
    }
    free(field);
}
/////////////////////////////////////////////////////
// Расчет значений /////////////////////////////////////////////////////////////////////////////////////////
double calc_potential(int x, int y, point_charge charge)
{
    double r = PIXEL_TO_METER_CONVERSION * sqrt(pow(abs(charge.x - x), 2) + pow(abs(charge.y - y), 2));
    return (Q_TO_nQ * ELECTRIC_COEFFICIENT * charge.value / r);
}

vect calc_elecrtic_strenght(int x, int y, point_charge charge)
{
    double r = PIXEL_TO_METER_CONVERSION * sqrt(pow(abs(charge.x - x), 2) + pow(abs(charge.y - y), 2));
    double full_vect = Q_TO_nQ * ELECTRIC_COEFFICIENT * charge.value / pow(r, 2);
    vect result;
    result.x_component = cos(x - charge.x) * full_vect;
    result.y_component = sin(y - charge.y) * full_vect;
    return result;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
