#ifndef DYNAMICATMOSPHERE_H
#define DYNAMICATMOSPHERE_H

#include "atmosphere.h"
#include "temperatureprofile.h"
#include <cmath>
#include <stdexcept>

class DynamicAtmosphere : public Atmosphere
{
public:
    /**
     * Конструктор динамической модели атмосферы.
     * @param baseDensity  базовая плотность (кг/м³)
     * @param basePressure базовое давление (Па)
     * @param gravity      ускорение свободного падения (м/с²)
     * @param tempProfile  указатель на профиль температуры
     * @param F107         дневной солнечный радиофлюкс (sfu)
     * @param F107A        81-дневное среднее солнечный радиофлюкс (sfu)
     */
    DynamicAtmosphere(double baseDensity, double basePressure, double gravity,
                      TemperatureProfile* tempProfile,
                      double F107, double F107A);

    virtual ~DynamicAtmosphere();

    /**
     * Возвращает плотность для заданной высоты (в метрах) и времени.
     * Параметр time не используется.
     */
    long double getDensity(double altitude, double time) const override;

private:
    int FindBand(double height) const;

    TemperatureProfile* mTempProfile;
    double mF107;
    double mF107A;

    int mAltitudeBands;
    double* mRefHeight;
    double* mRefDensity;
    double* mScaleHeight;
};

#endif // DYNAMICATMOSPHERE_H
