#include "atmosphere.h"

Atmosphere::Atmosphere(double baseDensity, double basePressure, double gravity)
    : mBaseDensity(baseDensity),
    mBasePressure(basePressure),
    mGravity(gravity)
{
}

Atmosphere::~Atmosphere()
{
}
