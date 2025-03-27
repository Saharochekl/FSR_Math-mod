#ifndef EXPONENTIALATMOSPHERE_H
#define EXPONENTIALATMOSPHERE_H
#include "atmosphere.h"
#include "temperatureprofile.h"
#include <cmath>

class ExponentialAtmosphere : public Atmosphere
{
public:

    ExponentialAtmosphere(double baseDensity, double basePressure, double gravity,
                          TemperatureProfile* tempProfile);
    ExponentialAtmosphere(double baseDensity, double basePressure, double gravity);
    virtual ~ExponentialAtmosphere();
    long double getDensity(double altitude, double time) const override;


private:

    void SetConstants();
    int FindBand(double height) const;

    TemperatureProfile* mTempProfile;  ///< Указатель на профиль температуры (может быть nullptr).
    int mAltitudeBands;
    double* mRefHeight;
    double* mRefDensity;
    double* mScaleHeight;
};

#endif // EXPONENTIALATMOSPHERE_H
