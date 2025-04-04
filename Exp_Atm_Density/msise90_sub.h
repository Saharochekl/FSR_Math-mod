#ifndef MSISE90_SUB_H
#define MSISE90_SUB_H

#ifdef __cplusplus
extern "C" {
#endif
extern float sod;
extern int   yd;
extern float f107;
extern float f107a;
extern float ap[7];

#ifdef USE_64_BIT_LONGS
void gtd6_(long int*, float*, float*, float*, float*, float*, float*, float*, float*, long int*, float*, float*);
#else
void gtd6_(int*, float*, float*, float*, float*, float*, float*, float*, float*, int*, float*, float*);
#endif

#ifdef __cplusplus
}
#endif

#endif // MSISE90_SUB_H

