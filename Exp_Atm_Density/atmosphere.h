#ifndef ATMOSPHERE_H
#define ATMOSPHERE_H


class Atmosphere
{
public:

    Atmosphere(double baseDensity, double basePressure, double gravity);


    virtual ~Atmosphere();

    virtual long double getDensity(double altitude, double time) const = 0;

protected:
    double mBaseDensity;
    double mBasePressure;
    double mGravity;
};
#endif // ATMOSPHERE_H
