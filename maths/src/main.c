#include "io.h"
#include "math_module.h"

void test_1(void)
{
    double **field = NULL;
    int rows = 100;
    int cols = 100;
    point_charge charges[3] = {{9, 49, 0.01}, {60, 29, -0.02}, {60, 69, 0.01}};
    int count_of_charges = 3;

    calculate_potential_field(rows, cols, charges, count_of_charges, &field);
    log_file(rows, cols, field, "test_1.txt");
    free_potential_field(rows, field);
}

void test_2(void)
{
    double **field = NULL;
    int rows = 100;
    int cols = 100;
    point_charge charges[4] = {{4, 4, 0.01}, {94, 94, 0.01}, {4, 94, -0.01}, {94, 4, -0.01}};
    int count_of_charges = 4;

    calculate_potential_field(rows, cols, charges, count_of_charges, &field);
    log_file(rows, cols, field, "test_2.txt");
    free_potential_field(rows, field);
}

void test_3(void)
{
    double **field = NULL;
    int rows = 100;
    int cols = 100;
    point_charge charges[2] = {{49, 24, 0.01}, {49, 74, -0.01}};
    int count_of_charges = 2;

    calculate_potential_field(rows, cols, charges, count_of_charges, &field);
    log_file(rows, cols, field, "test_3.txt");
    free_potential_field(rows, field);
}

int main(void)
{
    test_1();
    test_2();
    test_3();
    return OK;
}