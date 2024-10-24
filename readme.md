Компиляция под линукс
Библиотека
gcc -std=c99 -Wall -fPIC -c math_module.c
gcc -shared -o libpotential_field.so math_module.o -lm
На винде аналогично (может быть без -fPIC)