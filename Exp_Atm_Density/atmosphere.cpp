#include "atmosphere.h"

Atmosphere::Atmosphere(double baseDensity, double basePressure, double gravity)
    : mBaseDensity(baseDensity),
    mBasePressure(basePressure),
    mGravity(gravity)
{
}

Atmosphere::~Atmosphere()
{
    // Если нужны какие-то действия при удалении, можно добавить здесь.
}
