#ifndef MSISE90_SUB_H
#define MSISE90_SUB_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __SKIP_MSISE90__
#ifdef USE_64_BIT_LONGS
void gtd6_(long int*, float*, float*, float*, float*, float*, float*, float*, float*, long int*, float*, float*);
#else
void gtd6_(int*, float*, float*, float*, float*, float*, float*, float*, float*, int*, float*, float*);
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif // MSISE90_SUB_H

