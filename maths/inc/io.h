#ifndef IO_H__
#define IO_H__

#include "definitions_math.h"

#ifdef __cplusplus
extern "C" {
#endif

void log_file(int rows, int cols, double **field, const char *fn);

#ifdef __cplusplus
}
#endif

#endif // IO_H__
