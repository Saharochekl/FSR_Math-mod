#include "temperatureprofile.h"

TemperatureProfile::TemperatureProfile()
{

}

double TemperatureProfile::getTemperature(double altitude) const
{
    // Преобразуем высоту из метров в километры
    double alt_km = altitude / 1000.0;

    if (alt_km < 0.0)
        return 288.15;

    if (alt_km < 11.0)
        return 288.15 - 6.5 * alt_km;
    else if (alt_km < 20.0)
        return 216.65;
    else if (alt_km < 47.0)
        return 216.65 + 2.0 * (alt_km - 20.0);
    else if (alt_km < 51.0)
        return 270.65;
    else if (alt_km < 86.0)
        return 270.65 - 3.0 * (alt_km - 51.0);
    else if (alt_km < 100.0)
    {
        double T86 = 270.65 - 3.0 * (86.0 - 51.0); // ~165.65
        double slope = (200.0 - T86) / (100.0 - 86.0);
        return T86 + slope * (alt_km - 86.0);
    }
    else if (alt_km < 300.0)
    {
        double slope = (1500.0 - 200.0) / (300.0 - 100.0);
        return 200.0 + slope * (alt_km - 100.0);
    }
    else
        return 1500.0;
}
