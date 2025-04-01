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

/**
 * @brief Заполняет массивы константами (как в Vallado)
 */
/*void ExponentialAtmosphere::SetConstants()
{
    mRefHeight   = new double[mAltitudeBands];
    mRefDensity  = new double[mAltitudeBands];
    mScaleHeight = new double[mAltitudeBands];

    // Примерная таблица для 28 диапазонов
    mRefHeight[0]    = 0.0;    mRefDensity[0]   = 1.225;       mScaleHeight[0]  = 7.249;
    mRefHeight[1]    = 25.0;   mRefDensity[1]   = 3.899e-2;    mScaleHeight[1]  = 6.349;
    mRefHeight[2]    = 30.0;   mRefDensity[2]   = 1.774e-2;    mScaleHeight[2]  = 6.682;
    mRefHeight[3]    = 40.0;   mRefDensity[3]   = 3.972e-3;    mScaleHeight[3]  = 7.554;
    mRefHeight[4]    = 50.0;   mRefDensity[4]   = 1.057e-3;    mScaleHeight[4]  = 8.382;
    mRefHeight[5]    = 60.0;   mRefDensity[5]   = 3.206e-4;    mScaleHeight[5]  = 7.714;
    mRefHeight[6]    = 70.0;   mRefDensity[6]   = 8.770e-5;    mScaleHeight[6]  = 6.549;
    mRefHeight[7]    = 80.0;   mRefDensity[7]   = 1.905e-5;    mScaleHeight[7]  = 5.799;
    mRefHeight[8]    = 90.0;   mRefDensity[8]   = 3.396e-6;    mScaleHeight[8]  = 5.382;
    mRefHeight[9]    = 100.0;  mRefDensity[9]   = 5.297e-7;    mScaleHeight[9]  = 5.877;
    mRefHeight[10]   = 110.0;  mRefDensity[10]  = 9.661e-8;    mScaleHeight[10] = 7.263;
    mRefHeight[11]   = 120.0;  mRefDensity[11]  = 2.438e-8;    mScaleHeight[11] = 9.473;
    mRefHeight[12]   = 130.0;  mRefDensity[12]  = 8.484e-9;    mScaleHeight[12] = 12.636;
    mRefHeight[13]   = 140.0;  mRefDensity[13]  = 3.845e-9;    mScaleHeight[13] = 16.149;
    mRefHeight[14]   = 150.0;  mRefDensity[14]  = 2.070e-9;    mScaleHeight[14] = 22.523;
    mRefHeight[15]   = 180.0;  mRefDensity[15]  = 5.464e-10;   mScaleHeight[15] = 29.740;
    mRefHeight[16]   = 200.0;  mRefDensity[16]  = 2.789e-10;   mScaleHeight[16] = 37.105;
    mRefHeight[17]   = 250.0;  mRefDensity[17]  = 7.248e-11;   mScaleHeight[17] = 45.546;
    mRefHeight[18]   = 300.0;  mRefDensity[18]  = 2.418e-11;   mScaleHeight[18] = 53.628;
    mRefHeight[19]   = 350.0;  mRefDensity[19]  = 9.518e-12;   mScaleHeight[19] = 53.298;
    mRefHeight[20]   = 400.0;  mRefDensity[20]  = 3.725e-12;   mScaleHeight[20] = 58.515;
    mRefHeight[21]   = 450.0;  mRefDensity[21]  = 1.585e-12;   mScaleHeight[21] = 60.828;
    mRefHeight[22]   = 500.0;  mRefDensity[22]  = 6.967e-13;   mScaleHeight[22] = 63.822;
    mRefHeight[23]   = 600.0;  mRefDensity[23]  = 1.454e-13;   mScaleHeight[23] = 71.835;
    mRefHeight[24]   = 700.0;  mRefDensity[24]  = 3.614e-14;   mScaleHeight[24] = 88.667;
    mRefHeight[25]   = 800.0;  mRefDensity[25]  = 1.170e-14;   mScaleHeight[25] = 124.64;
    mRefHeight[26]   = 900.0;  mRefDensity[26]  = 5.245e-15;   mScaleHeight[26] = 181.05;
    mRefHeight[27]   = 1000.0; mRefDensity[27]  = 3.019e-15;   mScaleHeight[27] = 268.00;
}*/


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
