#include "exponentialatmosphere.h"
#include "atmosphereconst.h"
#include <stdexcept>


ExponentialAtmosphere::ExponentialAtmosphere(double baseDensity, double basePressure, double gravity,
                                             TemperatureProfile* tempProfile)
    : Atmosphere(baseDensity, basePressure, gravity),
    mTempProfile(tempProfile),
    mAltitudeBands(28),
    mRefHeight(nullptr),
    mRefDensity(nullptr),
    mScaleHeight(nullptr)
{
    AtmosphereConstants::SetConstants(mAltitudeBands, mRefHeight, mRefDensity, mScaleHeight);
}



ExponentialAtmosphere::ExponentialAtmosphere(double baseDensity, double basePressure, double gravity)
    : Atmosphere(baseDensity, basePressure, gravity),
    mTempProfile(nullptr),
    mAltitudeBands(28),
    mRefHeight(nullptr),
    mRefDensity(nullptr),
    mScaleHeight(nullptr)
{
    AtmosphereConstants::SetConstants(mAltitudeBands, mRefHeight, mRefDensity, mScaleHeight);
}


ExponentialAtmosphere::~ExponentialAtmosphere()
{
    delete[] mScaleHeight;
    delete[] mRefHeight;
    delete[] mRefDensity;
}


int ExponentialAtmosphere::FindBand(double height) const
{
    int index = mAltitudeBands - 1;
    for (int i = 0; i < mAltitudeBands - 1; ++i)
    {
        if (height < mRefHeight[i+1])
        {
            index = i;
            break;
        }
    }
    return index;
}


long double ExponentialAtmosphere::getDensity(double altitude, double /*time*/) const
{
    if (altitude < 0.0)
        throw std::runtime_error("Altitude is below zero");
    double alt_km = altitude / 1000.0;
    int band = FindBand(alt_km);
    double density = mRefDensity[band] * std::exp( -(alt_km - mRefHeight[band]) / mScaleHeight[band] );
    return density;
}
