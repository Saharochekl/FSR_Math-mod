#include "dynamic_atmosphere.h"
#include "atmosphereconst.h"

DynamicAtmosphere::DynamicAtmosphere(double baseDensity, double basePressure, double gravity,
                                     TemperatureProfile* tempProfile,
                                     double F107, double F107A)
    : Atmosphere(baseDensity, basePressure, gravity),
    mTempProfile(tempProfile),
    mF107(F107),
    mF107A(F107A),
    mAltitudeBands(28),
    mRefHeight(nullptr),
    mRefDensity(nullptr),
    mScaleHeight(nullptr)
{
    AtmosphereConstants::SetConstants(mAltitudeBands, mRefHeight, mRefDensity, mScaleHeight);
}

DynamicAtmosphere::~DynamicAtmosphere()
{
    delete[] mScaleHeight;
    delete[] mRefHeight;
    delete[] mRefDensity;
}

int DynamicAtmosphere::FindBand(double height) const
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

long double DynamicAtmosphere::getDensity(double altitude, double /*time*/) const
{
    if (altitude < 0.0)
        throw std::runtime_error("Altitude is below zero");

    // Переводим высоту из метров в км
    double alt_km = altitude / 1000.0;

    int band = FindBand(alt_km);
    // Базовая плотность по экспоненциальной модели
    double baseDensityValue = mRefDensity[band] * std::exp( -(alt_km - mRefHeight[band]) / mScaleHeight[band] );

    // Вычисляем динамический корректирующий множитель.
    // Используем среднее значение F10.7: (mF107 + mF107A)/2.
    // Номинальное значение можно принять равным, например, 150 sfu.
    double nominal = 150.0;
    double averageF107 = (mF107 + mF107A) / 2.0;
    double k = 0.01; // коэффициент чувствительности (подбирается экспериментально)

    double correctionFactor = 1.0 + k * (averageF107 - nominal) / nominal;

    return baseDensityValue * correctionFactor;
}
