#ifndef EXP_ATMOSPHERE_H
#define EXP_ATMOSPHERE_H

#include "atmosphere.h"


class exp_atmosphere : public atmosphere
{
public:
    exp_atmosphere();
protected:

   double *scaleHeight;
   double *refHeight;
   double *refDensity;
   int altitudeBands;
   bool smoothDensity;

   virtual void SetConstants();
   int FindBand(Real height);
};

#endif
