#include "math_module.h"

void readfile(const char *filename, int *x, int *y, point_charge *charges, int *count)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        return;
    }
    fscanf(file, "%d %d", x, y);
    fgetc(file);
    fscanf(file, "%d", count);
    fgetc(file);
    for (int i = 0; i < *count; i++)
    {
        fscanf(file, "%d %d %lf", &charges[i].x, &charges[i].y, &charges[i].value);
        fgetc(file);
    }
    fclose(file);
}

void writefile(const char *filename, double **field, int rows, int cols)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        return;
    }
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            fprintf(file, "%.10lf ", field[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

double **lol_kek_cheburek_wrapper(int rows, int cols, point_charge *charges, int count)
{
    double **potential_field = NULL;
    printf("calculate\n");
    calculate_potential_field(rows, cols, charges, count, &potential_field);
    printf("write\n");
    // writefile(out_file, potential_field, rows, cols);
    printf("free\n");
    // free_potential_field(rows, potential_field);
    return potential_field;
}

void *calculate_potential_field_thread(void *arg)
{
    potential_field_data *data = (potential_field_data *)arg;
    for (int i = data->start_row; i < data->end_row; i++)
    {
        for (int j = 0; j < data->cols; j++)
        {
            for (int c = 0; c < data->count_of_charges; c++)
            {
                if (is_position_allowed(i, j, data->charges[c]))
                {
                    (*data->field)[i][j] += calc_potential(i, j, data->charges[c]);
                }
            }
        }
    }
    return NULL;
}

void *calculate_electric_field_thread(void *arg)
{
    electric_field_data *data = (electric_field_data *)arg;
    for (int i = data->start_row; i < data->end_row; i++)
    {
        for (int j = 0; j < data->cols; j++)
        {
            for (int c = 0; c < data->count_of_charges; c++)
            {
                if (is_position_allowed(i, j, data->charges[c]))
                {
                    vect cc = calc_elecrtic_strenght(i, j, data->charges[c]);
                    (*data->field)[i][j].x_component += cc.x_component;
                    (*data->field)[i][j].y_component += cc.y_component;
                }
            }
        }
    }
    return NULL;
}

int calculate_potential_field(int rows, int cols, point_charge *charges, int count_of_charges, double ***field)
{
    if (alloc_potential_field(rows, cols, field) != OK)
        return ALLOC_ERR;

    int num_threads = 16;
    pthread_t threads[num_threads];
    potential_field_data data[num_threads];
    int rows_per_thread = rows / num_threads;

    for (int t = 0; t < num_threads; t++)
    {
        data[t].start_row = t * rows_per_thread;
        data[t].end_row = (t == num_threads - 1) ? rows : (t + 1) * rows_per_thread;
        data[t].cols = cols;
        data[t].charges = charges;
        data[t].count_of_charges = count_of_charges;
        data[t].field = field;
        pthread_create(&threads[t], NULL, calculate_potential_field_thread, &data[t]);
    }

    for (int t = 0; t < num_threads; t++)
    {
        pthread_join(threads[t], NULL);
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

    int num_threads = 16;
    pthread_t threads[num_threads];
    electric_field_data data[num_threads];
    int rows_per_thread = rows / num_threads;

    for (int t = 0; t < num_threads; t++)
    {
        data[t].start_row = t * rows_per_thread;
        data[t].end_row = (t == num_threads - 1) ? rows : (t + 1) * rows_per_thread;
        data[t].cols = cols;
        data[t].charges = charges;
        data[t].count_of_charges = count_of_charges;
        data[t].field = field;
        pthread_create(&threads[t], NULL, calculate_electric_field_thread, &data[t]);
    }

    for (int t = 0; t < num_threads; t++)
    {
        pthread_join(threads[t], NULL);
    }

    return OK;
}

int alloc_potential_field(int rows, int cols, double ***field)
{
    double **f = NULL;
    f = (double **)calloc(rows, sizeof(double *));
    if (f == NULL)
        return ALLOC_ERR;

    for (int i = 0; i < rows; i++)
    {
        f[i] = (double *)calloc(cols, sizeof(double));
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
    }

    *field = f;

    return OK;
}

int is_position_allowed(int x, int y, point_charge charge)
{
    double r = sqrt(pow(abs(charge.x - x), 2) + pow(abs(charge.y - y), 2));
    return ((r < RADIUS_RESTRICTION) ? 0 : 1);
}

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

