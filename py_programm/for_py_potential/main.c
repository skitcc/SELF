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

void writefile(const char *filename, double value)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        return;
    }
    fprintf(file, "%.10lf\n", value);
    fclose(file);
}

int main(int argc, char **argv)
{
    char *in_file = argv[1];
    char *out_file = argv[2];
    int x, y, count;
    point_charge charges[1000];

    readfile(in_file, &x, &y, charges, &count);
    double res = calculate_single_point_potential(x, y, charges, count);
    writefile(out_file, res);
    return 0;
}
