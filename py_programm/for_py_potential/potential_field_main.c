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

int main(int argc, char **argv)
{
    char *in_file = argv[1];
    char *out_file = argv[2];
    int rows, cols, count;
    point_charge charges[1000];

    readfile(in_file, &rows, &cols, charges, &count);
    double **potential_field = NULL;
    calculate_potential_field(rows, cols, charges, count, &potential_field);
    writefile(out_file, potential_field, rows, cols);
    free_potential_field(rows, potential_field);
    return 0;
}
