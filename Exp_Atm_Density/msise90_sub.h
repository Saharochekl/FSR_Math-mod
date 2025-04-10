#ifndef MSISE90_SUB_H
#define MSISE90_SUB_H

#include <stdio.h>
#include "f2c.h"

#ifdef __cplusplus
extern "C" {
#endif
extern float sod;
extern int   yd;
extern float f107;
extern float f107a;
extern float ap[7];

#ifdef USE_64_BIT_LONGS
int gtd6_(long int*, float*, float*, float*, float*, float*, float*, float*, float*, long int*, float*, float*);
#else
int gtd6_(int*, float*, float*, float*, float*, float*, float*, float*, float*, integer*, float*, float*);
#endif

#ifdef __cplusplus
}
#endif

#endif // MSISE90_SUB_H

