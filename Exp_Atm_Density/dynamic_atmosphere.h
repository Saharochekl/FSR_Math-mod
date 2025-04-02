#ifndef DYNAMICATMOSPHERE_H
#define DYNAMICATMOSPHERE_H

#include "atmosphere.h"
#include "temperatureprofile.h"
#include <cmath>
#include <stdexcept>
#include <ctime>
#include <iomanip>


struct UTC_time{
    double time_h, time_m, time_s;
    int year, month, day;

};
UTC_time convertUtcToLocalSolarTime(const std::time_t utcTime, double longitude);


struct coordinates{
    long double attitude, latitude, longitude;
};

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
     *
     */
    double getSOD(double time_h, double time_m, double time_s);
    long double getDensity(double altitude, double time) const override;
    coordinates CalculateGeodetics(const double* position, UTC_time time);
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

long double utcToJulianDate(const UTC_time& utc);

#endif // DYNAMICATMOSPHERE_H
