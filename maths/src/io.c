#include "io.h"

void log_file(int rows, int cols, double **field, const char *fn)
{
    FILE *fp = fopen(fn, "w");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            fprintf(fp, "%.2lf\t", field[i][j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}
